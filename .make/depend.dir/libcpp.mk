DEPEND_TARGETS += libcpp.recursive
ECHO_TARGETS += libcpp.echo
libcpp.recursive:
	@echo SUPER_MAKE_DIR=/volumes/llama/home/alpine/libraries/libstring/               >> /volumes/llama/home/alpine/libraries/libcpp/.make/super
	@echo SUPER_MAKE_CONFIG_DIR=/volumes/llama/home/alpine/libraries/libstring/.make >> /volumes/llama/home/alpine/libraries/libcpp/.make/super
	cd /volumes/llama/home/alpine/libraries/libcpp/ && make recursive && make install
	-rm /volumes/llama/home/alpine/libraries/libcpp/.make/super
libcpp.echo:
	@echo SUPER_MAKE_DIR=/volumes/llama/home/alpine/libraries/libstring/               >> /volumes/llama/home/alpine/libraries/libcpp/.make/super
	@echo SUPER_MAKE_CONFIG_DIR=/volumes/llama/home/alpine/libraries/libstring/.make >> /volumes/llama/home/alpine/libraries/libcpp/.make/super
	cd /volumes/llama/home/alpine/libraries/libcpp/ && make echo
	-rm /volumes/llama/home/alpine/libraries/libcpp/.make/super
