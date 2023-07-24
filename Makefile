LDir=libs
SDir=source

build: ${SDir}/*

	@set -e

	@if test ! -d ${LDir}; then \
		mkdir -p ${LDir}; \
	fi

	@echo "\nBuilding..."

	@TOTAL=$$(ls -A "${PWD}/${SDir}/" | wc -l); \
	CURRENT=0; \
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
		fi \
	done

	@echo "Successfully done.\n"


clean: ${LDir}/

	@if test -d ${LDir}; then \
		echo "\nCleaning..." ; \
		rm -rf ${LDir} ; \
		echo "Successfully done.\n" ; \
	fi
