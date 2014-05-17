Decor3D
==============

Compilation
--------------

	mkdir build
	cmake ../src
	make
	
	# to place executable in ../bin
	# or just execute ./decor3d
	make install

	# to create an Eclipse project, can be adapted for other IDEs
	cd ..
	mkdir ide-gen
	cd ide-gen
	cmake -G"Eclispe CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ..
	cd ..
	rm -rf ide-gen



