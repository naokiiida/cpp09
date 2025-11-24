# Meta Makefile for cpp09: Run `make run` in all ex0* directories

EX_DIRS := $(sort $(wildcard ex0*))
RUN_TARGETS := $(addsuffix /run,$(EX_DIRS))

.PHONY: all run $(EX_DIRS) $(RUN_TARGETS)

all: run

run: $(RUN_TARGETS)

$(RUN_TARGETS):
	@echo "---- Running make run in $(@D) ----"
	@$(MAKE) -C $(@D) run

# Optionally, add a clean target to clean all subdirectories
clean:
	@for dir in $(EX_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: clean
