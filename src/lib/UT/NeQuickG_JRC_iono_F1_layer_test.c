/**
 * NeQuickG F1 layer Unit test
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC_UT
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_iono_F1_layer_test.h"

#include "NeQuickG_JRC_iono_F1_layer.h"
#include "NeQuickG_JRC_macros.h"

#define NEQUICK_G_JRC_IONO_F1_LAYER_TEST_VECTORS_COUNT (3)
#define NEQUICK_G_JRC_IONO_F1_LAYER_CRIT_FREQ_EPSILON (1e-5)

typedef struct NeQuickG_iono_F1_layer_test_st {
  double_t E_layer;
  double_t F2_layer;
  double_t freq_expected;
} NeQuickG_iono_F1_layer_test_t;

static const NeQuickG_iono_F1_layer_test_t
  NeQuickG_iono_F1_layer_test_vector[NEQUICK_G_JRC_IONO_F1_LAYER_TEST_VECTORS_COUNT] =
{
  {
    2.409694,
    6.575613,
    3.373572
  },
  {
    4.178533,
    15.977583,
    5.849947
  },
  {
    1.851210,
    6.394916,
    0.000000
  },
};

static bool NeQuickG_iono_F1_layer_new(void) {

  for (size_t i = 0x00; i < NEQUICK_G_JRC_IONO_F1_LAYER_TEST_VECTORS_COUNT; i++) {

    layer_t F1;
    F1_layer_get_critical_freq_MHz(
      &F1,
      NeQuickG_iono_F1_layer_test_vector[i].E_layer,
      NeQuickG_iono_F1_layer_test_vector[i].F2_layer);

    if (!THRESHOLD_COMPARE(
        F1.critical_frequency_MHz,
        NeQuickG_iono_F1_layer_test_vector[i].freq_expected,
        NEQUICK_G_JRC_IONO_F1_LAYER_CRIT_FREQ_EPSILON)) {
      LOG_ERROR("F1 layer. Critical frequency is not the expected.");
      return false;
    }
  }
  return true;
}

bool NeQuickG_iono_F1_layer_test(void) {
  return NeQuickG_iono_F1_layer_new();
}

#undef NEQUICK_G_JRC_IONO_F1_LAYER_CRIT_FREQ_EPSILON
#undef NEQUICK_G_JRC_IONO_F1_LAYER_TEST_VECTORS_COUNT
