LDir=libs
SDir=source

.ONESHELL:

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
