from conan import ConanFile
from conan.tools.env import Environment

class CollectorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
       self.requires("cpr/1.12.0")
       self.requires("pugixml/1.15")
       self.requires("spdlog/1.15.3")
       self.requires("gtest/1.17.0")
       self.requires("libpqxx/7.10.2")
    
    def layout(self):
        build_folder = str(self.settings.build_type).lower()
        self.folders.build = f"build/{build_folder}"
        self.folders.generators = self.folders.build
        
