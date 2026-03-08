#/*****************************************************************
# * Copyright (C) 2017 Robert Valler - All rights reserved.
# *
# * This file is part of the project: <insert project name here>
# *
# * This project can not be copied and/or distributed
# * without the express permission of the copyright holder
# *****************************************************************/

from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout

class CommsStackRecipe(ConanFile):
    name = "net_stack"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
#        self.requires("boost/1.88.0")
#        self.requires("asio/1.36.0")
#        self.requires("gtest/1.17.0")
        self.requires("protobuf/6.32.1")
#        self.requires("logger/1.0.1")
#        self.requires("openssl/3.6.0")
#        self.requires("libssh/0.11.3")
#        self.requires("libssh2/1.11.1")
#        self.requires("libxml2/2.15.0")


    def layout(self):
        cmake_layout(self)

