LDir=libs
SDir=source

.ONESHELL:


# build: compile

# 	@echo "\033[0;32mBuilding...\033[0m"
# 	@mkdir build
# 	@mv libs build/ && cp -r include/* build
# 	@g++ -shared -o build/libpxe.so build/${LDir}/*
# 	@if [ -d build/PXE ] && [ -f build/libpxe.so ]; then \
#         echo "\033[1A\033[11C \033[0;34mDone.\033[0m"; \
#     else \
#         echo "\033[0;31mBuild failed.\033[0m"; \
#     fi



build: ${SDir}/*

	@if test ! -d ${LDir}; then \
		mkdir -p ${LDir}; \
	fi

	@echo "\nBuilding..."

	STATUS=1

	@TOTAL=$$(ls -A "${PWD}/${SDir}/" | wc -l); \
	CURRENT=0; \
	export STATUS; \
	for file in ${PWD}/${SDir}/* ; do \
		filename="$${file##*/}" ; \
		echo "Compiling $${file} file..." ; \
		g++ -c $${file} -I${PWD}/include/ ; \
		if [ -f "$${filename%.*}.o" ]; then \
			CURRENT=$$(($${CURRENT}+1)) ; \
			PERCENT=$$(($${CURRENT}*100/$${TOTAL})) ; \
			mv $${filename%.*}.o ${LDir}/ ; \
			echo "\033[0;32mBuilding $${PWD}/${LDir}/$${filename%.*}.o file...\033[0m $$PERCENT%" ; \
		else \
			echo "\033[0;31mError: unable to build $${PWD}/${LDir}/$${filename%.*}.o\033[0m" ; \
			STATUS=0; \
		fi \
	done 

	@export STATUS; \
	if [ "$${STATUS}" = 1 ]; then \
		echo "\n\033[0;34mSuccessfully done.\033[0m\n"; \
	else \
		echo "\n\033[0;31mFaild to build!\033[0m\n"; \
	fi


clean: ${LDir}/

	@if test -d ${LDir}; then \
		echo "\nCleaning..." ; \
		rm -rf ${LDir} ; \
		echo "Successfully done.\n" ; \
	fi

rebuild: clean build