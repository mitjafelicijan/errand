VERSION = 0.1.0

OS := $(shell uname -s | tr A-Z a-z)
ARCH := $(shell uname -p)

erd: main.go
	go build -ldflags "-X main.Version=$(VERSION) -s -w" -o erd

install:
	cp erd ~/.local/bin/

package: erd
	tar -czvf errand-$(VERSION)-$(OS)-$(ARCH).tar.gz erd
