NAME := exodus
CONTAINER_NAME := reasonml
REPO := kogai/$(CONTAINER_NAME)
SRC := $(shell find ./src -type f -name '*.re')
DIST := lib/js/src/main.bs.js

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
	docker build -t $(CONTAINER_NAME) .
	docker tag $(CONTAINER_NAME) $(REPO)
	docker push $(REPO)

.PHONY: clean
clean:
	rm -rf flow/_build
	rm -rf _build
