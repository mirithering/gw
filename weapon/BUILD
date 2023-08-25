cc_library(
    name = "weapon",
    srcs = [
        "weapon.cpp",
    ],
    hdrs = [
        "dagger.h",
        "sword.h",
        "weapon.h",
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
        "dagger_test.cpp",
        "sword_test.cpp",
        "weapon_test.cpp",
    ],
    deps = [
        "weapon",
        "@com_google_googletest//:gtest_main",
    ],
)
