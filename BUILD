load(
    "//bazel:vpg_build_system.bzl",
    "vpg_package",
    "vpg_cc_library",
)

vpg_package()

vpg_cc_library(
    name = "vpg_lib",
    hdrs = glob(["include/vpg/**/*.h"]),
    srcs = glob(["src/*.cc"]),
    strip_include_prefix = "include",
)

