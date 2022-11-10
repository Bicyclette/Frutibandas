from conans import ConanFile
from conan.tools.system.package_manager import Apt, Dnf, PacMan, Pkg, Yum, Zypper
from conans.tools import SystemPackageTool

class Frutibandas(ConanFile):
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

    def imports(self):
        self.copy("*.dll", "", "./build/bin")
        self.copy("*.dylib*", "", "./build/bin")
        self.copy("*.so*", "", "./build/bin")
