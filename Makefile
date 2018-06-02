NAME := exodus
OPAM_VERSION := 4.06.0
DIR := _build/default/src
PARSER_NAME := parser_flow.cmxa
PARSER_DIR := $(shell pwd)/flow/_build/src/parser
PARSER := $(PARSER_DIR)/$(PARSER_NAME)
PKGS := "sedlex,wtf8"
SRC_DIRS := "src flow/src/parser"
SRC := $(shell find ./src -type f -name '*.ml')
OCB_FLAGS := -use-ocamlfind -Is $(SRC_DIRS) -pkgs $(PKGS)
OCB := ocamlbuild $(OCB_FLAGS)

# all: $(DIR)/main.exe $(PARSER)
all: $(NAME).native

$(NAME).native: $(PARSER) $(SRC)
	$(OCB) $(NAME).native

# $(DIR)/main.exe: $(SRC)
# 	jbuilder build src/main.exe

$(PARSER):
	@cd flow/src/parser && \
	make

.PHONY: run
run: $(NAME).native
	./$(NAME).native
	# jbuilder exec src/main.exe
# run: $(SRC)
# 	jbuilder exec src/main.exe

.PHONY: install
install:
	opam install -y \
				sedlex \
				wtf8 \
				dtoa

.PHONY: init
init:
	opam init --comp $(OPAM_VERSION)
	eval `opam config env`
	opam install -y \
				jbuilder \
				merlin \
				ocp-indent
	opam user-setup install

.PHONY: pin
pin:
	opam pin add parser_flow "$(pwd)/flow/src/parser"

.PHONY: clean
clean:
	rm -rf flow/_build
	rm -rf _build
