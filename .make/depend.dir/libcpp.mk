DEPEND_TARGETS += libcpp.recursive
ECHO_TARGETS += libcpp.echo
libcpp.recursive:
	@echo SUPER_MAKE_DIR=/home/lidali/alpine/libraries/libstring/               >> /home/lidali/alpine/libraries/libcpp/.make/super
	@echo SUPER_MAKE_CONFIG_DIR=/home/lidali/alpine/libraries/libstring/.make >> /home/lidali/alpine/libraries/libcpp/.make/super
	cd /home/lidali/alpine/libraries/libcpp/ && make recursive && make install
	-rm /home/lidali/alpine/libraries/libcpp/.make/super
libcpp.echo:
	@echo SUPER_MAKE_DIR=/home/lidali/alpine/libraries/libstring/               >> /home/lidali/alpine/libraries/libcpp/.make/super
	@echo SUPER_MAKE_CONFIG_DIR=/home/lidali/alpine/libraries/libstring/.make >> /home/lidali/alpine/libraries/libcpp/.make/super
	cd /home/lidali/alpine/libraries/libcpp/ && make echo
	-rm /home/lidali/alpine/libraries/libcpp/.make/super
