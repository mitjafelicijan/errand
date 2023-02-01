erd: main.go
	go build -ldflags "-X main.Version=1.0.7 -s -w" -o erd

install:
	cp erd ~/.local/bin/