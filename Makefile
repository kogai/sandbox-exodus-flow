NAME := exodus
REPO := kogai/$(NAME)
SRC := $(shell find ./src -type f -name '*.ml')

.PHONY: docker
docker:
	docker build -t $(NAME) .
	docker tag $(NAME) $(REPO)
	docker push $(REPO)

.PHONY: clean
clean:
	rm -rf flow/_build
	rm -rf _build
