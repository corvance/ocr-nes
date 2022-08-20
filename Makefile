# Convenience makefile.

clean:
	@cd core; make clean
	@cd frontend; make clean

core-debug:
	@cd core; make debug

core-release:
	@cd core; make release

debug: core-debug
	@cd frontend; make debug

release: core-release
	@cd frontend; make release
