/** NeQuickG auxiliary geometric defines and routines.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_geometry.h"

#include "NeQuickG_JRC_math_utils.h"

void angle_set(angle_t* const pAngle, double_t angle_degree) {

  pAngle->degree = angle_degree;
  pAngle->rad = NEQUICKG_JRC_DEGREE_TO_RAD(angle_degree);
  pAngle->sin = sin(pAngle->rad);
  pAngle->cos = cos(pAngle->rad);
}

void angle_set_rad(angle_t* const pAngle, double_t angle_rad) {
  pAngle->degree = NEQUICKG_JRC_RAD_TO_DEGREE(angle_rad);
  pAngle->rad = angle_rad;
  pAngle->sin = sin(pAngle->rad);
  pAngle->cos = cos(pAngle->rad);
}

void angle_set_fom_sin(angle_t* const pAngle) {
  pAngle->cos = get_cos_from_sin(pAngle->sin);
  pAngle->rad = atan2(pAngle->sin, pAngle->cos);
  pAngle->degree = NEQUICKG_JRC_RAD_TO_DEGREE(pAngle->rad);
}

void angle_set_fom_cos(angle_t* const pAngle) {
  pAngle->sin = get_sin_from_cos(pAngle->cos);
  pAngle->rad = atan2(pAngle->sin, pAngle->cos);
  pAngle->degree = NEQUICKG_JRC_RAD_TO_DEGREE(pAngle->rad);
}

void angle_restricted_set(
  angle_restricted_t* const pAngle, double_t rad) {
  pAngle->sin = sin(rad);
  pAngle->cos = cos(rad);
}
