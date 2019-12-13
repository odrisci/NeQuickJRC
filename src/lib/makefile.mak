#########################################
## NeQuickG_JRC library makefile
##
## Copyright 2019
## JRC.
#########################################

NEQUICK_LIB_DIRECTORY := $(CURRENT_FOLDER)

C_SEARCH_PATH += $(NEQUICK_LIB_DIRECTORY)

H_SEARCH_PATH += $(NEQUICK_LIB_DIRECTORY)/public
H_SEARCH_PATH += $(NEQUICK_LIB_DIRECTORY)/private


# the NeQuickG library sources
# only these sources must be deployed on the field
# (please respect alphabetical order)
SOURCEFILES += \
  ITU_R_P_371_8 \
  NeQuickG_JRC \
  NeQuickG_JRC_coordinates \
  NeQuickG_JRC_electron_density \
  NeQuickG_JRC_Gauss_Kronrod_integration \
  NeQuickG_JRC_geometry \
  NeQuickG_JRC_input_data \
  NeQuickG_JRC_interpolate \
  NeQuickG_JRC_iono_E_layer \
  NeQuickG_JRC_iono_F1_layer \
  NeQuickG_JRC_iono_F2_layer \
  NeQuickG_JRC_iono_F2_layer_fourier_coefficients \
  NeQuickG_JRC_iono_profile \
  NeQuickG_JRC_math_utils \
  NeQuickG_JRC_MODIP \
  NeQuickG_JRC_MODIP_grid \
  NeQuickG_JRC_ray \
  NeQuickG_JRC_ray_slant \
  NeQuickG_JRC_ray_vertical \
  NeQuickG_JRC_solar \
  NeQuickG_JRC_solar_activity \
  NeQuickG_JRC_TEC_integration \
  NeQuickG_JRC_time

ifeq ($(FTR_MODIP_CCIR_AS_CONSTANTS),1)
  SOURCEFILES += \
    NeQuickG_JRC_CCIR \
    NeQuickG_JRC_ccir11 \
    NeQuickG_JRC_ccir12 \
    NeQuickG_JRC_ccir13 \
    NeQuickG_JRC_ccir14 \
    NeQuickG_JRC_ccir15 \
    NeQuickG_JRC_ccir16 \
    NeQuickG_JRC_ccir17 \
    NeQuickG_JRC_ccir18 \
    NeQuickG_JRC_ccir19 \
    NeQuickG_JRC_ccir20 \
    NeQuickG_JRC_ccir21 \
    NeQuickG_JRC_ccir22

  C_SEARCH_PATH += $(NEQUICK_LIB_DIRECTORY)/CCIR
endif

# if debug target, add the unit test sources
# (the unit test will be executed as part of the test)
ifneq (, $(IS_DEBUG_TARGET_IF_NON_EMPTY))

  NEQUICK_LIB_UT_DIRECTORY := $(NEQUICK_LIB_DIRECTORY)/UT

  C_SEARCH_PATH += $(NEQUICK_LIB_UT_DIRECTORY)

  H_SEARCH_PATH += $(NEQUICK_LIB_UT_DIRECTORY)/public
  H_SEARCH_PATH += $(NEQUICK_LIB_UT_DIRECTORY)/private

  SOURCEFILES += \
    NeQuickG_JRC_API_test \
    NeQuickG_JRC_Az_test \
    NeQuickG_JRC_iono_E_layer_test \
    NeQuickG_JRC_iono_F1_layer_test \
    NeQuickG_JRC_iono_F2_layer_fourier_coefficients_test \
    NeQuickG_JRC_iono_F2_layer_test \
    NeQuickG_JRC_iono_layer_amplitudes_test \
    NeQuickG_JRC_iono_layer_thickness_test \
    NeQuickG_JRC_MODIP_test \
    NeQuickG_JRC_ray_test \
    NeQuickG_JRC_solar_test \
    NeQuickG_JRC_UT
endif

#libraries: the math lib
LIBS += -lm
