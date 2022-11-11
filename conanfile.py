from conans import ConanFile
import platform

class Frutibandas(ConanFile):

    os = platform.system()

    requires = (("assimp/5.2.2"),
                ("bullet3/3.24"),
                ("enet/1.3.17"),
                ("freetype/2.11.1"),
                ("glew/2.2.0"),
                ("libsndfile/1.0.31"),
                ("openal/1.21.1"),
                ("sdl/2.0.20"),
                ("zlib/1.2.13", "override"))

    generators = "cmake"

    def requirements(self):
        if(self.os == "Darwin"):
            self.requires("llvm-openmp/12.0.1")
    
    def imports(self):
        self.copy("*.dll", "", "./build/bin")
        self.copy("*.dylib*", "", "./build/bin")
        self.copy("*.so*", "", "./build/bin")

