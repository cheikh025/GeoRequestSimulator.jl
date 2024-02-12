module CppScenario
    using CxxWrap

    function init_lib()
        lib_path = get_libGenSC_path()
        # Assuming `CxxWrap.load` is the correct way to load the library in your version of CxxWrap
        CxxWrap.@wrapmodule(lib_path)
        return
    end

    function get_libGenSC_path()
        script_dir = @__DIR__
        return joinpath(script_dir, "../deps/cpp/build/libGenerateScenario.so")
    end

    init_lib()
end
