cc_library(
    name = "skills",
    srcs = [
        "attack/attack_skill.cpp",
        "attack/sword/barbarous_slice.cpp",
        "bonettis_defense.cpp",
    ],
    hdrs = [
        "attack/attack_skill.h",
        "attack/bow/arcing_shot.h",
        "attack/dagger/desperate_strike.h",
        "attack/sword/barbarous_slice.h",
        "attack/sword/crippling_slash.h",
        "attack/sword/gash.h",
        "attack/sword/jaizenju_strike.h",
        "attack/sword/pure_strike.h",
        "bonettis_defense.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//character",
        "//conditions",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = [
        "attack/bow/arcing_shot_test.cpp",
        "attack/dagger/desperate_strike_test.cpp",
        "attack/sword/barbarous_slice_test.cpp",
        "attack/sword/crippling_slash_test.cpp",
        "attack/sword/gash_test.cpp",
        "attack/sword/pure_strike_test.cpp",
        "bonettis_defense_test.cpp",
    ],
    deps = [
        "skills",
        "//character",
        "//test",
        "@com_google_googletest//:gtest_main",
    ],
)
