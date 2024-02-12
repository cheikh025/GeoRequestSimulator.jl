module CppScenario
    using CxxWrap
    # Define a function that returns the library path
    function get_libGenSC_path()
        script_dir = @__DIR__
        return joinpath(script_dir, "../deps/cpp/build/libGenerateScenario.so")
    end
    script_dir = @__DIR__
    @wrapmodule(joinpath(script_dir, "../deps/cpp/build/libGenerateScenario.so"))

    function __init__()
        @initcxx
    end
end
