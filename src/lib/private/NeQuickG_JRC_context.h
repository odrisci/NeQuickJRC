/**
 * NeQuick G JRC library context.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#ifndef NEQUICK_G_JRC_CONTEXT_H
#define NEQUICK_G_JRC_CONTEXT_H

#include "NeQuickG_JRC_input_data.h"
#include "NeQuickG_JRC_iono_profile.h"
#include "NeQuickG_JRC_MODIP.h"
#include "NeQuickG_JRC_solar_activity.h"
#include "NeQuickG_JRC_ray.h"

/** This structure contains the internal context
 * of the library.
 */
typedef struct NeQuickG_context_st {
  /** modip contex.*/
  modip_context_t modip;
  /** solar activity contex.*/
  solar_activity_t solar_activity;
  /** ionospheric profile contex.*/
  iono_profile_t profile;
  /** calculated ray contex.*/
  ray_context_t ray;
  /** input data contex.*/
  input_data_t input_data;
} NeQuickG_context_t;

#endif // NEQUICK_G_JRC_CONTEXT_H
