VERSION = 0.1.0

OS := $(shell uname -s | tr A-Z a-z)
ARCH := $(shell uname -p)

erd: main.go
	go build -ldflags "-X main.Version=$(VERSION) -s -w" -o erd

install:
	cp erd ~/.local/bin/

package: erd
	tar -czvf errand-$(VERSION)-$(OS)-$(ARCH).tar.gz erd

build: darwin-arm64 linux-amd64 linux-arm64

darwin-arm64:
	GOOS=darwin GOARCH=arm64 go build -ldflags "-X main.Version=$(VERSION) -s -w" -o erd
	tar -czvf errand-$(VERSION)-darwin-arm64.tar.gz erd
	md5sum errand-$(VERSION)-darwin-arm64.tar.gz > errand-$(VERSION)-darwin-arm64.md5
	rm erd

linux-amd64:
	GOOS=linux GOARCH=amd64 go build -ldflags "-X main.Version=$(VERSION) -s -w" -o erd
	tar -czvf errand-$(VERSION)-linux-amd64.tar.gz erd
	md5sum errand-$(VERSION)-linux-amd64.tar.gz > errand-$(VERSION)-linux-amd64.md5
	rm erd

linux-arm64:
	GOOS=linux GOARCH=arm64 go build -ldflags "-X main.Version=$(VERSION) -s -w" -o erd
	tar -czvf errand-$(VERSION)-linux-arm64.tar.gz erd
	md5sum errand-$(VERSION)-linux-arm64.tar.gz > errand-$(VERSION)-linux-arm64.md5
	rm erd
