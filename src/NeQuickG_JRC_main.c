/**
 * NeQuickG test program
 *
 * @author Angela Aragon-Angel (maria-angeles.aragon@ec.europa.eu)
 * @ingroup NeQuickG_JRC
 * @defgroup NeQuickG_JRC_main NeQuickG_JRC_main [NeQuickG_JRC_main]
 * @copyright Joint Research Centre (JRC), 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.<br>
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * @file
 */
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "NeQuickG_JRC.h"
#include "NeQuickG_JRC_exception.h"
#include "NeQuickG_JRC_input_data_std_stream.h"
#include "NeQuickG_JRC_input_data_stream.h"
#include "NeQuickG_JRC_macros.h"

#define NEQUICK_TEC_EXCEPTION -8
#ifdef FTR_UNIT_TEST
#define NEQUICK_UNIT_TEST_EXCEPTION -10
#endif
#define NEQUICK_G_JRC_TEC_EPSILON (5e-6)

static void to_std_output
  (NeQuickG_chandle nequick,
   double_t total_electron_content) {
  NeQuickG.input_data_to_std_output(nequick);
  printf(", TEC =  %12.5f\n", total_electron_content);
}

static
#if defined(__GNUC__) || defined(__GNUG__)
__attribute__((noreturn))
#endif
void usage(bool is_error) {
  printf("\nThis is NeQuickJRC version %.1f\n\n", NEQUICKG_VERSION);
  printf("Ionospheric Correction Algorithm for Galileo Single Frequency Users, 1.2 September 2016\n\n");
  printf("Usage: NeQuickJRC "
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
    "<modip file path>, <ccir files directory> "
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS
    "[Options] \n\n");

  printf("Options: \n\n");

  printf(" -c "
    "<a0> <a1> <a2> "
    "<month> <UT> "
    "<station.longitude> <station.latitude> <station.height> "
    "<satellite.longitude> <satellite.latitude> <satellite.height>\n");
  printf("\tReturns the STEC in the standard output\n\n");

  printf(" -f "
    "<a0> <a1> <a2> "
    "<input_file> "
    "<output_file>\n"
    "\tReturns the STEC in the ouput file\n"
    "\tinput file format is:\n"
    "\t\t<month> <UT> "
    "<station.longitude> <station.latitude> <station.height> "
    "<satellite.longitude> <satellite.latitude> <satellite.height>\n"
    "\toutput file format is:\n"
    "\t\tinput data + modip(degrees) + STEC\n\n");

  printf(" -h prints this message and exits\n\n");

  printf(" -j "
    "<input_file>\n"
    "\tCalculates and checks the STEC against the value given in the file\n"
    "\tfile format is:\n"
    "\tfirst line: <a0> <a1> <a2>\n"
    "\tfollowing lines:\n"
    "\t\t<month> <UT> "
    "<station.longitude> <station.latitude> <station.height> "
    "<satellite.longitude> <satellite.latitude> <satellite.height> "
    "<STEC expected>\n\n");


  printf("\nParameter\tDescription                  \tunit: \n\n");
  printf("STEC       \tSlant Total Electron Content\tTECU (10^16 electrons/m2) \n");
  printf("month      \tJanuary = 1, ..., December = 12\t---\n");
  printf("UT         \tUT time                        \thours\n");
  printf("latitude\tWGS-84 ellipsoidal latitude\tdegrees\n");
  printf("longitude\tWGS-84 ellipsoidal latitude\tdegrees\n");
  printf("height   \tWGS-84 ellipsoidal height\tmeters\n");
  printf("a0       \tFirst ionospheric coefficient  \tsfu (10^-22 W/(m2*Hz)\n");
  printf("a1       \tSecond ionospheric coefficient \tsfu/degree\n");
  printf("a2       \tThird ionospheric coefficient  \tsfu/degree^2\n");

  if (is_error) {
    exit(NEQUICK_USAGE_EXCEPTION);
  } else {
    exit(1);
  }
}

static bool check_result(
  NeQuickG_chandle nequick,
  const double_t total_electron_content_expected,
  const double_t total_electron_content,
  clock_t begin) {

  clock_t end = clock();
  double_t time_spent = (double_t)(end - begin) / CLOCKS_PER_SEC;

  if (isnormal(total_electron_content_expected)) {
    bool success = THRESHOLD_COMPARE(
      total_electron_content,
      total_electron_content_expected,
      NEQUICK_G_JRC_TEC_EPSILON);

    if (!success) {
      to_std_output(nequick, total_electron_content);
      printf(
        "TEC is not the expected, \
          expected = %12.5f, \
          calculated = %12.5f\n",
        total_electron_content_expected,
        total_electron_content);
      return false;
    }
  }

  printf("TEC = %12.5f, time spent (s) = %f\n",
        total_electron_content, time_spent);
  return true;
}

static bool get_TEC(
  const NeQuickG_handle nequick,
  const double_t total_electron_content_expected) {

  clock_t begin = clock();
  double_t total_electron_content;

  if (NeQuickG.get_total_electron_content(nequick, &total_electron_content) !=
      NEQUICK_OK) {
    return false;
  }

  if (check_result(
    nequick,
    total_electron_content_expected,
    total_electron_content,
    begin)) {
    return true;
  } else {
    return false;
  }
}

static int on_single_command(
  const NeQuickG_handle handle,
  int argc, char* argv[],
  int current_index) {

  bool ret = true;

  NeQuickG_input_data_std_stream_t stream;
  NeQuickG_input_data_std_stream_init(
    &stream,
    argc, argv, current_index,
    handle);

  while (NeQuickG_input_data_std_stream_get_next(&stream)) {
    if (!get_TEC(handle, stream.TEC_expected)) {
      ret = false;
    }
  }

  return (ret) ? NEQUICK_OK : NEQUICK_TEC_EXCEPTION;
}

static int on_file_command(
  const NeQuickG_handle handle,
  int argc, char* argv[],
  int current_index) {

  volatile int32_t ret = NEQUICK_OK;

  // coefficients in the command line
  {
    NeQuickG_input_data_std_stream_t stream;
    NeQuickG_input_data_std_stream_init(
      &stream,
      argc, argv, current_index,
      handle);
    NeQuickG_input_data_std_stream_get_coef(&stream);
    current_index = stream.offset;
  }

  // data in the input file
  char* pInput_data_file;
  {
    if (argc < (current_index + 1)) {
      NeQuick_exception_throw(
        NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_COUNT);
    }
    pInput_data_file = argv[current_index++];
  }

  char* pOutput_data_file;
  {
    if (argc < (current_index + 1)) {
      NeQuick_exception_throw(
        NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_COUNT);
    }
    pOutput_data_file = argv[current_index++];
  }

  NeQuickG_input_data_stream_t file_input_stream;
  {
    NeQuickG_input_data_stream_init(&file_input_stream);
    file_input_stream.az_in_header = false;
    file_input_stream.is_TEC_expected_field_present = false;
  }

  NeQuickG_input_data_stream_t file_output_stream;
  {
    NeQuickG_input_data_stream_init(&file_output_stream);
    file_output_stream.az_in_header = false;
    file_output_stream.read = false;
    file_output_stream.is_TEC_expected_field_present = true;
    file_output_stream.is_modip_field_present = true;
  }

  NEQUICK_TRY {

    NeQuickG_input_data_stream_open(
      &file_input_stream,
      pInput_data_file,
      handle);

    NeQuickG_input_data_stream_open(
      &file_output_stream,
      pOutput_data_file,
      handle);

    while (NeQuickG_input_data_stream_get_next(&file_input_stream)) {

      file_output_stream.modip = NeQuickG.get_modip(handle);

      ret = NeQuickG.get_total_electron_content(
            handle,
            &file_output_stream.TEC_expected);

      NeQuickG_input_data_stream_put(&file_output_stream);
    }

  } NEQUICK_CATCH_ALL_EXCEPTIONS {
    NeQuickG_input_data_stream_close(&file_input_stream);
    NEQUICK_RETHROW();
  } NEQUICK_END;

  NeQuickG_input_data_stream_close(&file_input_stream);

  return ret;
}

static int on_jrc_file(
  const NeQuickG_handle handle,
  const char* const pInput_data_file) {

  volatile bool ret = true;

  NeQuickG_input_data_stream_t stream;
  NeQuickG_input_data_stream_init(&stream);

  NEQUICK_TRY {

    NeQuickG_input_data_stream_open(
      &stream,
      pInput_data_file,
      handle);

    while (NeQuickG_input_data_stream_get_next(&stream)) {
      if (!get_TEC(handle, stream.TEC_expected)) {
        ret = false;
      }
    }
  } NEQUICK_CATCH_ALL_EXCEPTIONS {
    NeQuickG_input_data_stream_close(&stream);
    NEQUICK_RETHROW();
  } NEQUICK_END;

  NeQuickG_input_data_stream_close(&stream);

  return (ret) ? NEQUICK_OK : NEQUICK_TEC_EXCEPTION;
}

int main(int argc, char* argv[]) {

  volatile int ret = NEQUICK_OK;

  volatile NeQuickG_handle nequick = NEQUICKG_INVALID_HANDLE;

  NEQUICK_TRY {

#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
    char *pModip_file;
    char *pCCIR_directory;

    if (argc < 3) {
      if ((argc == 2) &&
          (argv[1][0] == '-') &&
          (strlen(argv[1]) == 2) &&
          (argv[1][1] == 'h')) {
        bool is_error = false;
        usage(is_error);
      } else {
        NeQuick_exception_throw(
          NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_COUNT);
      }
    }
#else
    if (argc == 1) {
      NeQuick_exception_throw(
        NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_COUNT);
    }
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

    int arg_index = 1;
#ifndef FTR_MODIP_CCIR_AS_CONSTANTS
    pModip_file = argv[arg_index++];
    pCCIR_directory = argv[arg_index++];
#endif // !FTR_MODIP_CCIR_AS_CONSTANTS

    if ((strlen(argv[arg_index]) != 2) && argv[arg_index][0] != '-') {
      NeQuick_exception_throw(
        NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_FORMAT);
    }

    NeQuickG_handle nequick_;
    ret = NeQuickG.init(pModip_file, pCCIR_directory, &nequick_);
    if (ret != NEQUICK_OK) {
      NeQuick_exception_throw(
        NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_FORMAT);
    }
    nequick = nequick_;

    char option = argv[arg_index++][1];

    switch (option) {

    // command line single parameters option
    case 'c':
      ret = on_single_command(nequick, argc, argv, arg_index);
      break;

    // file command without az coeff.
    case 'f':
      ret = on_file_command(nequick, argc, argv, arg_index);
      break;

    // JRC format file
    case 'j':
    {
      if (argc != (arg_index + 1)) {
        NeQuick_exception_throw(
          NEQUICK_USAGE_EXCEPTION, NEQUICK_USAGE_BAD_COUNT);
      }
      char* pInput_data_file = argv[arg_index++];
      ret = on_jrc_file(nequick, pInput_data_file);
      break;
    }
#ifdef FTR_UNIT_TEST
    // Unit test option
    case 'u':
    {
      //lint -e{746} not made in the presence of a prototype
      //due to FTR_MODIP_CCIR_AS_CONSTANTS trick
      bool ut_result = NeQuickG.unit_test(pModip_file, pCCIR_directory);
      if (ut_result) {
        printf("Unit test success\n");
        ret = NEQUICK_OK;
      } else {
        printf("Unit test error\n");
        ret = NEQUICK_UNIT_TEST_EXCEPTION;
      }
      break;
    }
#endif //FTR_UNIT_TEST
#ifdef FTR_MODIP_CCIR_AS_CONSTANTS
    case 'h' :
    {
      // to satisfy lint a boolean is declared
      bool is_error = false;
      usage(is_error);
    }
    //lint -fallthrough missing break is intended, usage exists !!!
#endif // FTR_MODIP_CCIR_AS_CONSTANTS
    default:
    {
      // to satisfy lint a boolean is declared
      bool is_error = true;
      usage(is_error);
    }
    }

  } NEQUICK_CATCH(NEQUICK_USAGE_EXCEPTION) {
    // to satisfy lint a boolean is declared
    bool is_error = false;
    usage(is_error);
  } NEQUICK_CATCH_ALL_EXCEPTIONS {
    ret = g_NeQuick_exception_type;
  } NEQUICK_END;

  NEQUICK_TRY {
    NeQuickG.close(nequick);
  } NEQUICK_CATCH_ALL_EXCEPTIONS {
    ret = INT16_MIN;
  } NEQUICK_END;

  return ret;
}

#undef NEQUICK_UNIT_TEST_EXCEPTION
#undef NEQUICK_TEC_EXCEPTION
#undef NEQUICK_G_JRC_TEC_EPSILON
