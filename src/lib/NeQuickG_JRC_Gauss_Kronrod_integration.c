/** TEC calculation along rays using Kronrod G<SUB>7</SUB>-K<SUB>15</SUB> adaptive quadrature method.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_Gauss_Kronrod_integration.h"

#include <stdio.h>

#include "NeQuickG_JRC_math_utils.h"
#include "NeQuickG_JRC_ray_slant.h"
#include "NeQuickG_JRC_ray_vertical.h"

/** Num of K15 pts for Kronrod integration */
#define NEQUICK_G_JRC_KRONROD_K15_POINT_COUNT (15)
/** Num of G7 pts for Kronrod integration */
#define NEQUICK_G_JRC_KRONROD_G7_POINT_COUNT (7)

/** Constants for where samples are taken in the Kronrod integration
 * (i.e. how far from the midpoint).
 */
#define NEQUICK_G_JRC_KRONROD_POINT_1 (0.207784955007898467600689403773245)
#define NEQUICK_G_JRC_KRONROD_POINT_2 (0.405845151377397166906606412076961)
#define NEQUICK_G_JRC_KRONROD_POINT_3 (0.586087235467691130294144838258730)
#define NEQUICK_G_JRC_KRONROD_POINT_4 (0.741531185599394439863864773280788)
#define NEQUICK_G_JRC_KRONROD_POINT_5 (0.864864423359769072789712788640926)
#define NEQUICK_G_JRC_KRONROD_POINT_6 (0.949107912342758524526189684047851)
#define NEQUICK_G_JRC_KRONROD_POINT_7 (0.991455371120812639206854697526329)

/** Constants for weights for K15 sample points in Kronrod integration */
#define NEQUICK_G_JRC_KRONROD_K15_WEIGHT_1 (0.022935322010529224963732008058970)
#define NEQUICK_G_JRC_KRONROD_K15_WEIGHT_2 (0.063092092629978553290700663189204)
#define NEQUICK_G_JRC_KRONROD_K15_WEIGHT_3 (0.104790010322250183839876322541518)
#define NEQUICK_G_JRC_KRONROD_K15_WEIGHT_4 (0.140653259715525918745189590510238)
#define NEQUICK_G_JRC_KRONROD_K15_WEIGHT_5 (0.169004726639267902826583426598550)
#define NEQUICK_G_JRC_KRONROD_K15_WEIGHT_6 (0.190350578064785409913256402421014)
#define NEQUICK_G_JRC_KRONROD_K15_WEIGHT_7 (0.204432940075298892414161999234649)
#define NEQUICK_G_JRC_KRONROD_K15_WEIGHT_8 (0.209482141084727828012999174891714)

/** Constants for weights for G7 sample points in Kronrod integration */
#define NEQUICK_G_JRC_KRONROD_G7_WEIGHT_1 (0.129484966168869693270611432679082)
#define NEQUICK_G_JRC_KRONROD_G7_WEIGHT_2 (0.279705391489276667901467771423780)
#define NEQUICK_G_JRC_KRONROD_G7_WEIGHT_3 (0.381830050505118944950369775488975)
#define NEQUICK_G_JRC_KRONROD_G7_WEIGHT_4 (0.417959183673469387755102040816327)

/** Set at what points the samples are used in integration process */
static const double_t xi[NEQUICK_G_JRC_KRONROD_K15_POINT_COUNT] = {
  -NEQUICK_G_JRC_KRONROD_POINT_7,
  -NEQUICK_G_JRC_KRONROD_POINT_6,
  -NEQUICK_G_JRC_KRONROD_POINT_5,
  -NEQUICK_G_JRC_KRONROD_POINT_4,
  -NEQUICK_G_JRC_KRONROD_POINT_3,
  -NEQUICK_G_JRC_KRONROD_POINT_2,
  -NEQUICK_G_JRC_KRONROD_POINT_1,
  0,
  NEQUICK_G_JRC_KRONROD_POINT_1,
  NEQUICK_G_JRC_KRONROD_POINT_2,
  NEQUICK_G_JRC_KRONROD_POINT_3,
  NEQUICK_G_JRC_KRONROD_POINT_4,
  NEQUICK_G_JRC_KRONROD_POINT_5,
  NEQUICK_G_JRC_KRONROD_POINT_6,
  NEQUICK_G_JRC_KRONROD_POINT_7
};

/** Set weights for K15 sample points */
static const double_t wi[NEQUICK_G_JRC_KRONROD_K15_POINT_COUNT] = {
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_1,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_2,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_3,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_4,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_5,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_6,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_7,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_8,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_7,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_6,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_5,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_4,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_3,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_2,
  NEQUICK_G_JRC_KRONROD_K15_WEIGHT_1
};

/** Set weights for G7 sample points */
static const double_t wig[NEQUICK_G_JRC_KRONROD_G7_POINT_COUNT] = {
  NEQUICK_G_JRC_KRONROD_G7_WEIGHT_1,
  NEQUICK_G_JRC_KRONROD_G7_WEIGHT_2,
  NEQUICK_G_JRC_KRONROD_G7_WEIGHT_3,
  NEQUICK_G_JRC_KRONROD_G7_WEIGHT_4,
  NEQUICK_G_JRC_KRONROD_G7_WEIGHT_3,
  NEQUICK_G_JRC_KRONROD_G7_WEIGHT_2,
  NEQUICK_G_JRC_KRONROD_G7_WEIGHT_1
};

static bool is_error_within_tolerance(
  const gauss_kronrod_context_t* const pContext,
  double_t K15_integration,
  double_t G7_integration) {

  return (
    (fabs((K15_integration - G7_integration) / K15_integration) <=
      pContext->tolerance) ||
    (fabs(K15_integration - G7_integration) <=
      pContext->tolerance));
}

static int32_t get_total_electron_content(
  NeQuickG_context_t* const pNequick_Context,
  double_t height_km,
  double_t* const pTotal_electron_content) {

  if (pNequick_Context->ray.is_vertical) {
    *pTotal_electron_content = ray_vertical_get_electron_density(
      pNequick_Context, height_km);
    return NEQUICK_OK;
  } else {
    return ray_slant_get_electron_density(
      pNequick_Context, height_km, pTotal_electron_content);
  }
}

int32_t Gauss_Kronrod_integrate(
  gauss_kronrod_context_t* const pContext,
  NeQuickG_context_t* const pNequick_Context,
  const double_t point_1_height_km,
  const double_t point_2_height_km,
  double_t* const pResult) {

  *pResult = 0.0;

  double_t mid_point = (point_1_height_km + point_2_height_km) / 2.0;
  double_t half_diff = (point_2_height_km - point_1_height_km) / 2.0;

  double_t K15_integration = 0.0;
  double_t G7_integration = 0.0;
  size_t G7_index = 0;

  size_t i;
  for (i = 0; i < NEQUICK_G_JRC_KRONROD_K15_POINT_COUNT; i++) {

    double_t height_km = mid_point + (half_diff * xi[i]);

    double_t total_electron_content;
    int32_t ret =
      get_total_electron_content(
        pNequick_Context, height_km, &total_electron_content);
    if (ret != NEQUICK_OK) {
      return ret;
    }

    K15_integration += (total_electron_content * wi[i]);

    if (IS_ODD(i)) {
      G7_integration += (total_electron_content * wig[G7_index]);
      G7_index++;
    }
  }

  K15_integration *= half_diff;
  G7_integration *= half_diff;

  if (is_error_within_tolerance(
    pContext,
    K15_integration,
    G7_integration)) {

    *pResult = K15_integration;
    return NEQUICK_OK;

  } else if (pContext->recursion_level == pContext->recursion_max) {

    *pResult = K15_integration;
    return NEQUICK_OK;

  } else {

    // Error not acceptable
    // split into two parts to improve accuracy
    // and try again.
    pContext->recursion_level++;

    double_t result;

    int32_t ret;

    ret = Gauss_Kronrod_integrate(
      pContext,
      pNequick_Context,
      point_1_height_km,
      point_1_height_km + half_diff,
      pResult);
    if (ret != NEQUICK_OK) {
      return ret;
    }

    ret = Gauss_Kronrod_integrate(
      pContext,
      pNequick_Context,
      point_1_height_km + half_diff,
      point_2_height_km,
      &result);

    *pResult += result;

    pContext->recursion_level--;

    return ret;
  }
}

#undef NEQUICK_G_JRC_KRONROD_K15_POINT_COUNT
#undef NEQUICK_G_JRC_KRONROD_G7_POINT_COUNT

#undef NEQUICK_G_JRC_KRONROD_POINT_1
#undef NEQUICK_G_JRC_KRONROD_POINT_2
#undef NEQUICK_G_JRC_KRONROD_POINT_3
#undef NEQUICK_G_JRC_KRONROD_POINT_4
#undef NEQUICK_G_JRC_KRONROD_POINT_5
#undef NEQUICK_G_JRC_KRONROD_POINT_6
#undef NEQUICK_G_JRC_KRONROD_POINT_7

#undef NEQUICK_G_JRC_KRONROD_K15_WEIGHT_1
#undef NEQUICK_G_JRC_KRONROD_K15_WEIGHT_2
#undef NEQUICK_G_JRC_KRONROD_K15_WEIGHT_3
#undef NEQUICK_G_JRC_KRONROD_K15_WEIGHT_4
#undef NEQUICK_G_JRC_KRONROD_K15_WEIGHT_5
#undef NEQUICK_G_JRC_KRONROD_K15_WEIGHT_6
#undef NEQUICK_G_JRC_KRONROD_K15_WEIGHT_7
#undef NEQUICK_G_JRC_KRONROD_K15_WEIGHT_8

#undef NEQUICK_G_JRC_KRONROD_G7_WEIGHT_1
#undef NEQUICK_G_JRC_KRONROD_G7_WEIGHT_2
#undef NEQUICK_G_JRC_KRONROD_G7_WEIGHT_3
#undef NEQUICK_G_JRC_KRONROD_G7_WEIGHT_4
