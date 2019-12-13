/** NeQuickG Third Order Interpolation function
 *  See 2.5.7.1.
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include <math.h>

/** Number of points used in the interpolation */
#define NEQUICK_G_JRC_INTERPOLATE_POINT_COUNT (4)

/** Third Order Interpolation function, See 2.5.7.1.
 *
 * Be P<SUB>1</SUB> = (-1,z<SUB>1</SUB>),
 * P<SUB>2</SUB> = (0,z<SUB>2</SUB>),
 * P<SUB>3</SUB> = (1,z<SUB>3</SUB>),
 * P<SUB>4</SUB> = (2,z<SUB>4</SUB>).
 * If P = (x,z<SUB>x</SUB>), to compute the interpolated value z<SUB>x</SUB> at the position x,
 * with x&isin;[0,1], the following algorithm is applied:<br>
 *
 * If |2x| < 2#NEQUICK_G_JRC_INTERPOL_EPSILON &rArr; z<SUB>x</SUB> = z<SUB>2</SUB><br>
 * else<br><CENTER>
 *  &delta; = 2x - 1<br>
 *  g<SUB>1</SUB> = z<SUB>3</SUB> + z<SUB>2</SUB><br>
 *  g<SUB>2</SUB> = z<SUB>3</SUB> - z<SUB>2</SUB><br>
 *  g<SUB>3</SUB> = z<SUB>4</SUB> + z<SUB>1</SUB><br>
 *  g<SUB>4</SUB> = (z<SUB>4</SUB> - z<SUB>1</SUB>) / 3<br>
 *  a<SUB>0</SUB> = 9g<SUB>1</SUB> - g<SUB>3</SUB><br>
 *  a<SUB>1</SUB> = 9g<SUB>2</SUB> - g<SUB>4</SUB><br>
 *  a<SUB>2</SUB> = g<SUB>3</SUB> - g<SUB>1</SUB><br>
 *  a<SUB>3</SUB> = g<SUB>4</SUB> - g<SUB>2</SUB><br>
 *  z<SUB>x</SUB> = (1/16) * (a<SUB>0</SUB> + a<SUB>1</SUB>&delta; + a<SUB>2</SUB>&delta;<SUP>2</SUP> + a<SUB>3</SUB>&delta;<SUP>3</SUP>)<br>
 * </CENTER>
 *
 * @param[in] interpol_points 4 interpolation points: z<SUB>1</SUB>, z<SUB>2</SUB>, z<SUB>3</SUB> and z<SUB>4</SUB>
 * @param[in] offset position x where x&isin;[0,1]
 * @return interpolated value z<SUB>x</SUB>
 */
extern double_t interpolation_third_order(
  const double_t interpol_points[NEQUICK_G_JRC_INTERPOLATE_POINT_COUNT],
  double_t offset);
