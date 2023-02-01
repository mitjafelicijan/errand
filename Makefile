erd: main.go
	go build -ldflags "-X main.Version=0.1.0 -s -w" -o erd

install:
	cp erd ~/.local/bin/