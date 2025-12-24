build-debug:
	cmake -G "Ninja Multi-Config" -S ./ -B build && \
	cmake --build build --target all --config debug

build-release: clean
	cmake -G "Ninja Multi-Config" -S ./ -B build && \
	cmake --build build --target VulkanTest --config release

packRelease: build-release
	cd build && \
	cpack

clean:
	del /Q /S build