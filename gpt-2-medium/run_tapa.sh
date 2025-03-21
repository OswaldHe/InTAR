tapac \
    -o opt.hw.xo \
    --platform xilinx_u280_xdma_201920_3 \
    --top opt_kernel \
    --work-dir opt-stage3.tapa \
    --connectivity hbm_config.ini \
    --enable-hbm-binding-adjustment \
    --enable-synth-util \
    --run-floorplan-dse \
    --min-area-limit 0.55 \
    --min-slr-width-limit 5000 \
    --max-slr-width-limit 19000 \
    --max-parallel-synth-jobs 16 \
    --floorplan-output opt-floorplan.tcl \
    kernel.cpp 