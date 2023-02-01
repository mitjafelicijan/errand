package main

import (
	"bufio"
	"errors"
	"flag"
	"fmt"
	"os"
	"os/exec"
	"regexp"
	"runtime"
	"strings"
	"time"
)

const version = "0.1.1"
const author = "Mitja Felicijan"

type Errand struct {
	ErrandFile        string
	Variables         []Variable
	Tasks             []Task
	IncludeSysEnvVars bool
	AssureCommands    []string
}

// Command type.
type Command struct {
	Expression string
	ShellType  string
}

// Task represents a task in the language.
type Task struct {
	Name        string
	Description string
	Commands    []Command
}

// Variable represents an environment variable in the language.
type Variable struct {
	Name  string
	Value string
}

// Check if Errand or Errandfile exists in the current directory.
func (e *Errand) CheckForDefaultFiles() error {
	var files = []string{"Errandfile", "Errand"}

	for _, file := range files {
		if _, err := os.Stat(file); err == nil {
			e.ErrandFile = file
			return nil
		}
	}

	return errors.New("no Errand files in the current directory")
}

// Parse the contents of a file
func (e *Errand) ParseFile() error {
	var tasks []Task
	var variables []Variable
	var env = make(map[string]string)

	f, err := os.Open(e.ErrandFile)
	if err != nil {
		return errors.New("file could not be opened")
	}
	defer f.Close()

	var currentTask *Task
	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		line := scanner.Text()
		line = strings.TrimSpace(line)

		// Skip empty lines and comments.
		if line == "" || strings.HasPrefix(line, "#") {
			continue
		}

		// Check if line is a assure declaration.
		if strings.HasPrefix(line, "@assure") {
			var assure = e.evalAssureLine(line)
			var commands []string = strings.Split(assure, " ")

			// Only append commands that are not already in the list.
			for _, cmd := range commands {
				for _, c := range e.AssureCommands {
					if c == cmd {
						continue
					}
				}
				e.AssureCommands = append(e.AssureCommands, cmd)
			}
			continue
		}

		// Check if line is a variable declaration.
		if strings.HasPrefix(line, "@var") {
			varName, varValue := e.evalVariableLine(line)
			variables = append(variables, Variable{varName, varValue})
			env[varName] = varValue
			continue
		}

		// Check if line is a task declaration.
		if strings.HasPrefix(line, "@task") && strings.HasSuffix(line, "does") {
			task := e.evalTaskLine(line)
			tasks = append(tasks, task)
			currentTask = &tasks[len(tasks)-1]
			continue
		}

		// If line has only end, then it is a task end.
		if strings.Compare(line, "end") == 0 && currentTask != nil {
			currentTask = nil
			continue
		}

		// Check if line is an env directive.
		if strings.HasPrefix(line, "@env") && currentTask == nil {
			e.IncludeSysEnvVars = e.evalEnvLine(line)
		}

		// Check if line is a command.
		if currentTask != nil {
			var cmd Command = Command{
				Expression: strings.TrimSpace(line[0:]),
				ShellType:  "shell",
			}

			currentTask.Commands = append(currentTask.Commands, cmd)
			continue
		}
	}

	if currentTask != nil {
		fmt.Println("Error: One of the tasks has not been closed with 'end'")
		os.Exit(1)
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}

	e.Variables = variables
	e.Tasks = tasks

	return nil
}

// Parses a @var declaration line.
func (e *Errand) evalVariableLine(line string) (string, string) {
	varLineRegex := regexp.MustCompile(`^@var\s+(\w+)\s+=\s+"([^"]+)"`)
	match := varLineRegex.FindStringSubmatch(line)
	return match[1], match[2]
}

// Parses a @task declaration line.
func (e *Errand) evalTaskLine(line string) Task {
	var task Task

	taskLineRegex := regexp.MustCompile(`^@task\s+([\w::]+)\s+"([\w\s]+)"\s+.*$`)
	match := taskLineRegex.FindStringSubmatch(line)

	task.Name = match[1]
	task.Description = match[2]
	task.Commands = []Command{}
	return task
}

// Evaluates @env directive line.
// TODO: Add that by default the system environment variables are included.
func (e *Errand) evalEnvLine(line string) bool {
	varLineRegex := regexp.MustCompile(`^@env\s+(on|off)$`)
	match := varLineRegex.FindStringSubmatch(line)

	if len(match) == 0 {
		fmt.Println("Error: Invalid @env directive")
		os.Exit(1)
	}

	return strings.Compare(match[1], "on") == 0
}

func (e *Errand) evalAssureLine(line string) string {
	varLineRegex := regexp.MustCompile(`^@assure\s+(.+)$`)
	match := varLineRegex.FindStringSubmatch(line)

	if len(match) == 0 {
		fmt.Println("Error: Invalid @assure directive")
		os.Exit(1)
	}

	return match[1]
}

// Replaces variables in commands with their values.
func (e *Errand) substituteVariables() error {
	for _, task := range e.Tasks {
		for i, _ := range task.Commands {
			for _, variable := range e.Variables {
				task.Commands[i].Expression = strings.ReplaceAll(
					task.Commands[i].Expression,
					fmt.Sprintf("$%s", variable.Name),
					variable.Value,
				)
			}
		}
	}
	return nil
}

// Loads the environment variables from the system
func (e *Errand) loadSystemEnvVars() error {
	for _, variable := range os.Environ() {
		var substitute bool = true
		variableParts := strings.Split(variable, "=")

		// Check if variable is already defined in the Errandfile
		for _, errandVariable := range e.Variables {
			if errandVariable.Name == variableParts[0] {
				substitute = false
				continue
			}
		}

		if substitute {
			e.Variables = append(e.Variables, Variable{variableParts[0], variableParts[1]})
		}
	}
	return nil
}

// Executes the task with the given name.
func (e *Errand) executeTask(taskName string) error {
	// Find the task.
	var task *Task = nil
	for _, possibleTask := range e.Tasks {
		if possibleTask.Name == taskName {
			task = &possibleTask
			break
		}
	}

	if task == nil {
		fmt.Println("Error: Task not found:", taskName)
		os.Exit(1)
	}

	cmd := exec.Command("/bin/bash")

	stdin, err := cmd.StdinPipe()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		fmt.Println("Error", err)
		os.Exit(1)
	}

	if err := cmd.Start(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	for _, command := range task.Commands {
		fmt.Println(command.Expression)
		fmt.Fprintln(stdin, command.Expression)
	}

	stdin.Close()

	scanner := bufio.NewScanner(stdout)
	for scanner.Scan() {
		fmt.Println(scanner.Text())
	}

	if err := cmd.Wait(); err != nil {
		fmt.Println("Error:", err)
		os.Exit(1)
	}

	return nil
}

func (e *Errand) assureCommandIsInstalled() error {
	var missingCommands []string = []string{}

	for _, cmd := range e.AssureCommands {
		_, err := exec.LookPath(cmd)
		if err != nil {
			missingCommands = append(missingCommands, cmd)
		}
	}

	if len(missingCommands) > 0 {
		fmt.Println("In order to use this project you will need additional tools installed.")
		fmt.Println("Error: The following commands are not installed:")

		for _, cmd := range missingCommands {
			fmt.Println("  * ", cmd)
		}

		os.Exit(1)
	}

	return nil
}

func main() {
	// Parse command line arguments.
	var versionFlag bool

	flag.BoolVar(&versionFlag, "v", false, "Display version")
	flag.Parse()

	// Display version and exit if -v flag is present.
	if versionFlag {
		fmt.Printf("Errand %s (built for %s)\n", version, runtime.GOARCH)
		fmt.Printf("A simple task runner for the command line.\n")
		fmt.Printf("Copyright (C) 2022-%d %s.\n", time.Now().Year(), author)
		fmt.Printf("License BSD 2-Clause: <https://opensource.org/licenses/BSD-2-Clause>\n")
		fmt.Printf("This is free software: you are free to change and redistribute it.\n")
		fmt.Printf("There is NO WARRANTY, to the extent permitted by law.\n")
		os.Exit(0)
	}

	// Store the remaining arguments as tasks.
	var targetTasks []string = flag.Args()

	// Create an Errand object.
	var err error
	var errand = Errand{}

	// Check if Errand or Errandfile exists in the current directory.
	err = errand.CheckForDefaultFiles()
	if err != nil {
		fmt.Println("Error:", err)
		os.Exit(0)
	}

	// Parse the contents of a file.
	err = errand.ParseFile()
	if err != nil {
		fmt.Println("Error:", err)
	}

	// Load system environment variables.
	if errand.IncludeSysEnvVars {
		err = errand.loadSystemEnvVars()
		if err != nil {
			fmt.Println("Error:", err)
		}
	}

	// Replace variables in commands.
	err = errand.substituteVariables()
	if err != nil {
		fmt.Println("Error:", err)
	}

	// Check if the required commands are installed.
	err = errand.assureCommandIsInstalled()
	if err != nil {
		fmt.Println("Error:", err)
	}

	// If no tasks were specified, display the list of available tasks.
	if len(targetTasks) == 0 {
		// Find longest task name.
		var longestTaskName int = 0
		for _, task := range errand.Tasks {
			if len(task.Name) > longestTaskName {
				longestTaskName = len(task.Name)
			}
		}

		// Display the list of available tasks.
		fmt.Println("Available tasks:")
		for _, task := range errand.Tasks {
			fmt.Printf("  %s%s%s\n", task.Name, strings.Repeat(" ", 5+longestTaskName-len(task.Name)), task.Description)
		}
		os.Exit(0)
	}

	// Execute the specified tasks.
	for _, taskName := range targetTasks {
		err = errand.executeTask(taskName)
		if err != nil {
			fmt.Println("Error:", err)
		}
	}

	// Exit successfully.
	os.Exit(0)
}
