using CxxWrap
prefix_path = CxxWrap.prefix_path()
print(prefix_path)
project_dir = @__DIR__ 
cpp_dir = joinpath(project_dir, "cpp")
build_dir = joinpath(cpp_dir, "build")

function run_build_script()
    # Create the build directory if it doesn't exist
    isdir(build_dir) || mkdir(build_dir)

    # Change the current working directory to the build directory
    cd(build_dir) do
        # Run cmake and make commands within the build directory
        run(`cmake -DCMAKE_PREFIX_PATH=$prefix_path $cpp_dir`)
        run(`make`)
    end
end

# Only run the build script if the library has not been built yet
if !isfile(joinpath(build_dir, "libGenerateScenario.so"))
    run_build_script()
end