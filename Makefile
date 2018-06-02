OPAM_VERSION := 4.05.0
DIR := _build/default/src
PARSER_NAME := parser_flow.cmxa
PARSER_DIR := $(shell pwd)/flow/_build/src/parser

all: $(DIR)/main.exe $(PARSER_DIR)/$(PARSER_NAME)

$(DIR)/main.exe:
	jbuilder build src/main.exe

$(PARSER_DIR)/$(PARSER_NAME):
	@cd flow/src/parser && \
	make

.PHONY: exec
exec:

.PHONY: install
install:
	opam install -y \
				jbuilder \
				sedlex \
				wtf8 \
				dtoa

.PHONY: init
init:
	opam init --comp $(OPAM_VERSION)
	eval `opam config env`
