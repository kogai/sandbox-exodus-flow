NAME := exodus
REPO := kogai/$(NAME)
SRC := $(shell find ./src -type f -name '*.re')
DIST := $(shell find ./src -type f -name '*.re' | sed 's/re/bs\.js/')

all: $(DIST)

$(DIST): $(SRC)
	npm run build

.PHONY: watch
watch: $(SRC)
	npx bsb -w

.PHONY: test
test: $(DIST)
	npx jest

.PHONY: test/watch
test/watch: $(DIST)
	npx jest --watch

.PHONY: docker
docker:
	docker build -t $(NAME) .
	docker tag $(NAME) $(REPO)
	docker push $(REPO)

.PHONY: clean
clean:
	rm -rf flow/_build
	rm -rf _build
