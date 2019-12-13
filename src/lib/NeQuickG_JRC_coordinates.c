/** NeQuick G JRC position coordinates
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include "NeQuickG_JRC_coordinates.h"

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_error.h"
#include "NeQuickG_JRC_geometry.h"

/** to convert meters to km*/
#define NEQUICK_G_POSITION_METERS_TO_KM(h)(h*1e-3)
/** to convert km to meters*/
#define NEQUICK_G_POSITION_KM_TO_METERS(h)(h*1e3)
/** Radius of the earth in km. 2.5.2 Constants used R<SUB>E</SUB>*/
#define NEQUICK_G_EARTH_RADIUS_IN_KM (6371.2)
/** Threshold used to checks if the path between two points (e.g. a satellite and a receiver) is vertical<br>
 * @see F.2.5.2
 * @see position_is_exterior
 */
#define NEQUICK_G_POSITION_HORIZONTAL_COORD_EPSILON (1.0e-5)

/** From the height get the radius in km
 * @param[in] p pointer to the position structure
 * @return the radius in km
 */
static double_t position_get_radius_km (
  const position_t* const p) {
  double_t height = NEQUICK_G_EARTH_RADIUS_IN_KM;
  if (p->height_units == NEQUICK_G_JRC_HEIGHT_UNITS_METERS) {
    height += NEQUICK_G_POSITION_METERS_TO_KM(p->height);
  } else {
    height += p->height;
  }
  return height;
}

int32_t position_set(
  position_t* const p,
  double_t longitude_degree,
  double_t latitude_degree,
  double_t height, uint_fast8_t height_units) {

  {
    if ((latitude_degree < NEQUICK_G_JRC_LAT_MIN_VALUE_DEGREE) ||
        (latitude_degree > NEQUICK_G_JRC_LAT_MAX_VALUE_DEGREE)) {
      NEQUICK_ERROR_RETURN(
        NEQUICK_ERROR_SRC_INPUT_DATA,
        NEQUICK_ERROR_CODE_BAD_LATITUDE,
        "latitude out of range: %lf (degrees), must be between -90.0 and 90.0",
        latitude_degree);
    }

    angle_set(&p->latitude, latitude_degree);
  }

  {
    p->longitude_org = longitude_degree;
    /* Longitude must be between 0.0 and 360.0 */
    longitude_degree =
      fmod(longitude_degree + NEQUICK_G_JRC_CIRCLE_DEGREES,
          NEQUICK_G_JRC_CIRCLE_DEGREES);

    angle_set(&p->longitude, longitude_degree);
  }

  {
    p->height = height;
    p->height_units = height_units;
    p->radius_km = position_get_radius_km(p);
  }
  return NEQUICK_OK;
}

void position_to_km(position_t* const p) {
  if (p->height_units == NEQUICK_G_JRC_HEIGHT_UNITS_METERS) {
    p->height = NEQUICK_G_POSITION_METERS_TO_KM(p->height);
    p->height_units =
      NEQUICK_G_JRC_HEIGHT_UNITS_KM;
  }
}

void position_to_meters(position_t* const p) {
 if (p->height_units == NEQUICK_G_JRC_HEIGHT_UNITS_KM) {
    p->height = NEQUICK_G_POSITION_KM_TO_METERS(p->height);
    p->height_units =
      NEQUICK_G_JRC_HEIGHT_UNITS_METERS;
  }
}

bool position_is_above(
  const position_t* const p1, const position_t* const p2) {
  return (
    (fabs(p2->latitude.degree - p1->latitude.degree) <
      NEQUICK_G_POSITION_HORIZONTAL_COORD_EPSILON) &&
    (fabs(p2->longitude.degree - p1->longitude.degree) <
      NEQUICK_G_POSITION_HORIZONTAL_COORD_EPSILON));
}

bool position_is_exterior(
  const double_t height_km) {
  return (height_km >= NEQUICK_G_EARTH_RADIUS_IN_KM);
}

double_t get_radius_from_height(const double_t height_km) {
  return (height_km + NEQUICK_G_EARTH_RADIUS_IN_KM);
}

double_t get_height_from_radius(const double_t radius_km) {
  if (radius_km < NEQUICK_G_EARTH_RADIUS_IN_KM) {
    return 0.0;
  }
  return (radius_km - NEQUICK_G_EARTH_RADIUS_IN_KM);
}

#undef NEQUICK_G_POSITION_HORIZONTAL_COORD_EPSILON
#undef NEQUICK_G_EARTH_RADIUS_IN_KM
#undef NEQUICK_G_POSITION_KM_TO_METERS
#undef NEQUICK_G_POSITION_METERS_TO_KM
