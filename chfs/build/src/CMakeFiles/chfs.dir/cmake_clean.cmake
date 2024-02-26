file(REMOVE_RECURSE
  "../lib/libchfs.a"
  "../lib/libchfs.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/chfs.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
