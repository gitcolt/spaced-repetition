add_executable(spaced_repetition
  main.c
  )

target_compile_options(spaced_repetition
  PRIVATE
  -g
    )

target_link_options(spaced_repetition
  PRIVATE
    "LINKER:-lncurses"
    )
