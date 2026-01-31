savedcmd_spot_high_res_timer.mod := printf '%s\n'   spot_high_res_timer.o | awk '!x[$$0]++ { print("./"$$0) }' > spot_high_res_timer.mod
