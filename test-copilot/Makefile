include ~/.makefiles/Arduino-check-syntax.mk

.PHONY: copilot-c99-codegen/copilot.c

copilot-c99-codegen/copilot.c:
	@rm -rf copilot-c99-codegen
	@stack runghc -- app/Main.hs

