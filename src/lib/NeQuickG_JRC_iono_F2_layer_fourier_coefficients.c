/** Coefficients for the foF2 and M(3000)F2 models recommended by the
 *  Comité Consultatif International des Radiocommunications (CCIR).
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_iono_F2_layer_fourier_coefficients.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "NeQuickG_JRC.h"
#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
#include "NeQuickG_JRC_CCIR.h"
#endif
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
#include "NeQuickG_JRC_error.h"
#endif
#include "NeQuickG_JRC_macros.h"
#include "NeQuickG_JRC_solar.h"

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
  #define ITU_F2_LAYER_COEFF_CCIR_FILE_NAME_BASE "ccir"
  #define ITU_F2_LAYER_COEFF_CCIR_FILE_NAME_EXT ".txt"
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

#define ITU_F2_LAYER_MAX_HARMONICS_CF2 ((ITU_F2_COEFF_MAX_ORDER - 1)/2)
#define ITU_F2_LAYER_MAX_HARMONICS_CM3 ((ITU_FM3_COEFF_MAX_ORDER - 1)/2)
#define ITU_F2_LAYER_MAX_HARMONICS_COEFF \
  (max(ITU_F2_LAYER_MAX_HARMONICS_CF2, ITU_F2_LAYER_MAX_HARMONICS_CM3))

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS

static int32_t coefficients_file_get_id(
  uint_fast8_t month,
  char* const file_id, size_t file_id_size) {
  if ((month < NEQUICK_G_JRC_MONTH_JANUARY) ||
      (month > NEQUICK_G_JRC_MONTH_DECEMBER)) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_F2_LAYER,
      NEQUICK_ERROR_CODE_BAD_MONTH,
      "Invalid month %u", month);
  }

  // 3.2.4.3.1 Store ccirXX.asc values
  // Select the file name to read: XX = m + 10
  // e.g. ccir21.asc for November
  month = (uint8_t)(month + 10);
  if (snprintf(file_id, file_id_size, "%2d", month) < 0) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_F2_LAYER,
      NEQUICK_ERROR_CODE_BAD_CCIR_FILE,
      "snprintf error for month %u", month);
  }
  return NEQUICK_OK;
}

static int32_t coefficients_file_open(
  const char* const pCCIR_folder,
  const char * const file_id, size_t file_id_size,
  FILE** ppFile) {
  size_t file_name_size =
    strlen(pCCIR_folder) +
    sizeof('/') +
    strlen(ITU_F2_LAYER_COEFF_CCIR_FILE_NAME_BASE) +
    file_id_size +
    strlen(ITU_F2_LAYER_COEFF_CCIR_FILE_NAME_EXT) +
    1;
  char *pFile_name = (char*)malloc(file_name_size);
  if (!pFile_name) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_F2_LAYER,
      NEQUICK_ERROR_CODE_BAD_CCIR_FILE,
      "Not enough resources to allocate file name on the heap");
  }
  {
    // to satisfy lint
    assert(pFile_name);
    memset(pFile_name, 0x00, file_name_size);
    strcpy(pFile_name, pCCIR_folder);
    if (pCCIR_folder[strlen(pCCIR_folder) - 1] != '/') {
      strcat(pFile_name, "/");
    }
    strcat(pFile_name, ITU_F2_LAYER_COEFF_CCIR_FILE_NAME_BASE);
    strcat(pFile_name, file_id);
    strcat(pFile_name, ITU_F2_LAYER_COEFF_CCIR_FILE_NAME_EXT);
  }
  *ppFile = fopen(pFile_name, "r");
  free(pFile_name);
  if (!(*ppFile)) {
    NEQUICK_ERROR_RETURN(
      NEQUICK_ERROR_SRC_F2_LAYER,
      NEQUICK_ERROR_CODE_BAD_CCIR_FILE,
      "Cannot open CCIR file folder = %s, id = %s", pCCIR_folder, file_id);
  }
  return NEQUICK_OK;
}

static int32_t coefficients_load_F2(
  FILE * const poFile,
  F2_layer_ionosonde_coefficients_t * const pCoeff) {
  size_t solar_activity_index;
  size_t degree;
  size_t order;

  for (solar_activity_index = 0;
   solar_activity_index < ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT;
   solar_activity_index++) {
    for (degree = 0; degree < ITU_F2_COEFF_MAX_DEGREE; degree++) {
      for (order = 0; order < ITU_F2_COEFF_MAX_ORDER; order++) {
        int ret = fscanf (poFile, "%lf",
            &pCoeff->F2[solar_activity_index][degree][order]);
        if (ret < 0) {
          NEQUICK_ERROR_RETURN(
            NEQUICK_ERROR_SRC_F2_LAYER,
            NEQUICK_ERROR_CODE_BAD_CCIR_STRUCTURE,
            "error scanning F2 coefficients using fscanf");
        }
      }
    }
  }
  return NEQUICK_OK;
}

static int32_t coefficients_load_FM3(
  FILE * const poFile,
  F2_layer_ionosonde_coefficients_t * const pCoeff) {
  size_t solar_activity_index;
  size_t degree;
  size_t order;

  for (solar_activity_index = 0;
   solar_activity_index < ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT;
   solar_activity_index++) {
    for (degree = 0; degree < ITU_FM3_COEFF_MAX_DEGREE; degree++) {
      for (order = 0; order < ITU_FM3_COEFF_MAX_ORDER; order++) {
        int ret = fscanf (poFile, "%lf",
            &pCoeff->Fm3[solar_activity_index][degree][order]);
        if (ret < 0) {
          NEQUICK_ERROR_RETURN(
            NEQUICK_ERROR_SRC_F2_LAYER,
            NEQUICK_ERROR_CODE_BAD_CCIR_STRUCTURE,
            "error scanning Fm3 coefficients using fscanf");
        }
      }
    }
  }
  return NEQUICK_OK;
}

static int32_t coefficients_load(
  FILE * const poFile,
  F2_layer_ionosonde_coefficients_t * const pCoeff) {
  int32_t ret = coefficients_load_F2(poFile, pCoeff);
  if (ret != NEQUICK_OK) {
    return ret;
  }
  return coefficients_load_FM3(poFile, pCoeff);
}

#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

int32_t F2_layer_fourier_coefficients_init(
  F2_layer_fourier_coeff_context_t* const pContext,
  const char* const pCCIR_folder) {

  NeQuickG_time_init(&pContext->time);

  {
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
    // store a copy of the CCIR folder
    {
      pContext->ionosonde.pFolder = malloc(strlen(pCCIR_folder) + 1);
      if (!pContext->ionosonde.pFolder) {
        NEQUICK_ERROR_RETURN(
          NEQUICK_ERROR_SRC_F2_LAYER,
          NEQUICK_ERROR_CODE_NO_RESOURCES,
        "Not enough resources to copy CCIR directory on the heap");
      }
      strcpy(pContext->ionosonde.pFolder, pCCIR_folder);
    }
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS
    pContext->interpolated.effective_sun_spot_count = 0.0;
    pContext->interpolated.valid = false;
    pContext->fourier.valid = false;
  }
  return NEQUICK_OK;
}

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
void F2_layer_fourier_coefficients_close(
  F2_layer_fourier_coeff_context_t* const pContext) {
  if (pContext->ionosonde.pFolder) {
    free(pContext->ionosonde.pFolder);
    pContext->ionosonde.pFolder = NULL;
  }
}

static int32_t load_ionosonde_coeff(
  F2_layer_fourier_coeff_context_t* const pContext,
  const NeQuickG_time_t* const pTime) {

  int32_t ret;

  char file_id[4 + 1];
  ret = coefficients_file_get_id(pTime->month, file_id, sizeof(file_id));
  if (ret != NEQUICK_OK) {
    return ret;
  }

  FILE* pFile = NULL;
  ret = coefficients_file_open(
    pContext->ionosonde.pFolder, file_id, strlen(file_id), &pFile);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  ret = coefficients_load(pFile, &pContext->ionosonde);

  if (pFile) {
    fclose(pFile);
  }
  return ret;
}
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

static int32_t set_parameters(
  F2_layer_fourier_coeff_context_t* const pContext,
  const NeQuickG_time_t* const pTime,
  const double_t Azr) {

  bool ionosonde_coeff_valid =
    NEQUICK_G_TIME_MONTH_EQUAL(
      pTime->month,
      pContext->time.month);

  if (ionosonde_coeff_valid) {
    pContext->interpolated.valid =
      solar_activity_is_effective_ionisation_level_equal(
        Azr,
        pContext->interpolated.effective_sun_spot_count*100.0);
    if (pContext->interpolated.valid) {
      pContext->fourier.valid =
        NeQuickG_time_utc_is_equal(
          pTime->utc,
          pContext->time.utc);
    } else {
      pContext->interpolated.effective_sun_spot_count = Azr / 100.0;
      pContext->fourier.valid = false;
    }
    if (!pContext->fourier.valid) {
      pContext->time.utc = pTime->utc;
    }
  } else {
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
    int32_t ret = load_ionosonde_coeff(pContext, pTime);
    if (ret != NEQUICK_OK) {
      return ret;
    }
#endif
    pContext->time = *pTime;
    pContext->interpolated.effective_sun_spot_count = Azr / 100.0;
    pContext->interpolated.valid = false;
    pContext->fourier.valid = false;
  }
  return NEQUICK_OK;
}

/**
 * AF2 elements are calculated by linear combination of the elements of F2.
 */
static int32_t interpolate_F2(
  F2_layer_fourier_coeff_context_t* const pContext) {

  int32_t ret;

#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
  F2_coefficient_array_t* pF2;
  ret = CCIR_get_ionosonde_F2(pContext->time.month, &pF2);
  if (ret != NEQUICK_OK) {
    return ret;
  }
#else
  ret = NEQUICK_OK;
#endif // FTR_MODIP_CCIR_AS_CONSTANTS

  size_t degree;
  size_t order;
  for (degree = 0; degree < ITU_F2_COEFF_MAX_DEGREE; degree++) {
    for (order = 0; order < ITU_F2_COEFF_MAX_ORDER; order++) {
      pContext->interpolated.F2[degree][order] =
#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
        pF2
#else
        pContext->ionosonde.F2
#endif
        [ITU_F2_LAYER_COEFF_LOW_SOLAR_ACTIVITY_IDX]
          [degree][order] * (1.0 - pContext->interpolated.effective_sun_spot_count) +
#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
        pF2
#else
        pContext->ionosonde.F2
#endif
        [ITU_F2_LAYER_COEFF_HIGH_SOLAR_ACTIVITY_IDX]
          [degree][order] * pContext->interpolated.effective_sun_spot_count;
    }
  }
  return ret;
}

/**
 * Am3 elements are calculated by linear combination of the elements of Fm3:
 */
static int32_t interpolate_Fm3(
  F2_layer_fourier_coeff_context_t* const pContext) {

  int32_t ret;

#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
  Fm3_coefficient_array_t* pFm3;
  ret = CCIR_get_ionosonde_Fm3(pContext->time.month, &pFm3);
  if (ret != NEQUICK_OK) {
    return ret;
  }
#else
  ret = NEQUICK_OK;
#endif // FTR_MODIP_CCIR_AS_CONSTANTS

  size_t degree;
  size_t order;
  for (degree = 0; degree < ITU_FM3_COEFF_MAX_DEGREE; degree++) {
    for (order = 0; order < ITU_FM3_COEFF_MAX_ORDER; order++) {
      pContext->interpolated.Fm3[degree][order] =
#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
        pFm3
#else
        pContext->ionosonde.Fm3
#endif

        [ITU_F2_LAYER_COEFF_LOW_SOLAR_ACTIVITY_IDX]
          [degree][order] * (1.0 - pContext->interpolated.effective_sun_spot_count) +
#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
        pFm3
#else
        pContext->ionosonde.Fm3
#endif
        [ITU_F2_LAYER_COEFF_HIGH_SOLAR_ACTIVITY_IDX]
          [degree][order] * pContext->interpolated.effective_sun_spot_count;
    }
  }
  return ret;
}

#ifndef FTR_UNIT_TEST
static
#endif
int32_t F2_layer_fourier_coefficients_interpolate(
  F2_layer_fourier_coeff_context_t* const pContext,
  const NeQuickG_time_t * const pTime,
  const double_t Azr) {

  int32_t ret = set_parameters(pContext, pTime, Azr);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  if (!pContext->interpolated.valid) {

    ret = interpolate_F2(pContext);
    if (ret != NEQUICK_OK) {
      return ret;
    }

    ret = interpolate_Fm3(pContext);
    if (ret != NEQUICK_OK) {
      return ret;
    }

    pContext->interpolated.valid = true;
  }
  return NEQUICK_OK;
}

static void get_harmonics(
  const NeQuickG_time_t * const pTime,
  double_t * const pSin,
  double_t * const pCos) {
  double_t solar_longitude_rad =
    solar_get_longitude(pTime);

  //sin(nA) = sin[(n-1)A + A] with sin(A + B) = sin(A)cos(B) + cos(A)sin(B),
  //cos(nA) = cos[(n-1)A + A] with cos(A + B) = cos(A)cos(B) - sin(A)sin(B).
  size_t i = 0;
  {
    pSin[i] = sin(solar_longitude_rad);
    pCos[i] = cos(solar_longitude_rad);
    i++;
  }
  //lint -e{506} ITU_F2_LAYER_MAX_HARMONICS_COEFF is not constant, as lint means
  while (i < ITU_F2_LAYER_MAX_HARMONICS_COEFF) {
    size_t prev_index = i - 1;
    pSin[i] = pSin[prev_index] * pCos[0] + pCos[prev_index] * pSin[0];
    pCos[i] = pCos[prev_index] * pCos[0] - pSin[prev_index] * pSin[0];
    i++;
  }
}

static void get_fourier_coefficients_CF2(
  const F2_layer_fourier_coeff_context_t* const pContext,
  const double_t * const pSin,
  const double_t * const pCos,
  F2_fourier_coefficient_array_t CF2) {
  for (size_t i = 0x00; i < ITU_F2_COEFF_MAX_DEGREE; i++) {
    CF2[i] = pContext->interpolated.F2[i][0];
    for (size_t j = 0x00; j < ITU_F2_LAYER_MAX_HARMONICS_CF2; j++) {
      size_t order = 2 * (j + 1);
      CF2[i] +=
        (pSin[j] * pContext->interpolated.F2[i][order - 1]) +
        (pCos[j] * pContext->interpolated.F2[i][order]);
    }
  }
}

static void get_fourier_coefficients_Cm3(
  const F2_layer_fourier_coeff_context_t* const pContext,
  const double_t * const pSin,
  const double_t * const pCos,
  Fm3_fourier_coefficient_array_t Cm3) {
  for (size_t i = 0x00; i < ITU_FM3_COEFF_MAX_DEGREE; i++) {
    Cm3[i] = pContext->interpolated.Fm3[i][0];
    for (size_t j = 0x00; j < ITU_F2_LAYER_MAX_HARMONICS_CM3; j++) {
      size_t order = 2 * (j + 1);
      Cm3[i] +=
        (pSin[j] * pContext->interpolated.Fm3[i][order - 1]) +
        (pCos[j] * pContext->interpolated.Fm3[i][order]);
    }
  }
}

static
void F2_layer_fourier_coefficients_get_impl(
  F2_layer_fourier_coeff_context_t* const pContext) {

  //lint -e{506} ITU_F2_LAYER_MAX_HARMONICS_COEFF is not constant, as lint means
  double_t sinus_terms[ITU_F2_LAYER_MAX_HARMONICS_COEFF];
  //lint -e{506} ITU_F2_LAYER_MAX_HARMONICS_COEFF is not constant, as lint means
  double_t cosinus_terms[ITU_F2_LAYER_MAX_HARMONICS_COEFF];

  get_harmonics(&pContext->time, sinus_terms, cosinus_terms);

  get_fourier_coefficients_CF2(
    pContext,
    sinus_terms, cosinus_terms,
    pContext->fourier.CF2);

  get_fourier_coefficients_Cm3(
    pContext,
    sinus_terms, cosinus_terms,
    pContext->fourier.Cm3);
}

int32_t F2_layer_fourier_coefficients_get(
  F2_layer_fourier_coeff_context_t* const pContext,
  const NeQuickG_time_t * const pTime,
  const solar_activity_t* const pSolar_activity) {

  int32_t ret = F2_layer_fourier_coefficients_interpolate(
    pContext, pTime, pSolar_activity->effective_sun_spot_count);
  if (ret != NEQUICK_OK) {
    return ret;
  }

  if (!pContext->fourier.valid) {
    F2_layer_fourier_coefficients_get_impl(pContext);
    pContext->fourier.valid = true;
  }

  return NEQUICK_OK;
}

#undef ITU_F2_LAYER_MAX_HARMONICS_COEFF
#undef ITU_F2_LAYER_MAX_HARMONICS_CM3
#undef ITU_F2_LAYER_MAX_HARMONICS_CF2

#undef ITU_F2_LAYER_COEFF_CCIR_FILE_NAME_BASE
#undef ITU_F2_LAYER_COEFF_CCIR_FILE_NAME_EXT
