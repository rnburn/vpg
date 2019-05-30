load(
    "//bazel:vpg_build_system.bzl",
    "vpg_cc_library",
    "vpg_package",
)

vpg_package()

# From commit a575536abe20b58c48863600f8a71e93b4052b81
vpg_cc_library(
    name = "catch2",
    hdrs = [
        "catch.hpp",
    ],
    is_3rd_party = True,
)

vpg_cc_library(
    name = "main_lib",
    srcs = [
        "catch_main.cpp",
    ],
    deps = [
        ":catch2",
    ],
    is_3rd_party = True,
)
