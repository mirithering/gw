cc_library(
    name = "armor",
    hdrs = [
        "armor.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//base",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = [
        "armor_test.cpp",
    ],
    deps = [
        "armor",
        "@com_google_googletest//:gtest_main",
    ],
)
