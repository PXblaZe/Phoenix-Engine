LDir=libs
SDir=source


build: ${SDir}/*

	@set -e

	@if test ! -d ${LDir}; then \
		mkdir -p ${LDir}; \
	fi

	@echo "\nBuilding..."

	@for file in ${PWD}/${SDir}/* ; do \
		filename="$${file##*/}" ; \
		echo "Compiling $${file} file..." ; \
		g++ -c $${file} -I${PWD}/include/ ; \
		echo "Building $${PWD}/${LDir}/$${filename%.*}.o file..." ; \
		mv $${filename%.*}.o ${LDir}/ ; \
		echo ; \
	done

	@echo "Successfully done.\n"


clean: ${LDir}/

	@if test -d ${LDir}; then \
		echo "\nCleaning..." ; \
		rm -rf ${LDir} ; \
		echo "Successfully done.\n" ; \
	fi