/*!
 * NeQuickG (NeQuick Galileo) JRC Readme file
 *
 * @copyright European Union, 2019<br>
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1.2,
 *  https://joinup.ec.europa.eu/sites/default/files/custom-page/attachment/eupl_v1.2_en.pdf <br>
 *  Questions? Submit your query at https://www.gsc-europa.eu
 * Version: #NEQUICKG_VERSION<br>
 *
 * Release date: <b>10/12/2019</b>
 *
 * <h3>Background</h3>
 * Galileo is the European global navigation satellite system providing a highly accurate and global positioning service
 * under civilian control. Galileo, and in general current GNSS, are based on the broadcasting of electromagnetic ranging
 * signals in the L frequency band. Those satellite signals suffer from a number of impairments when propagating
 * through the Earth’s ionosphere. Receivers  operating  in  single  frequency  mode  may  use  the  single  frequency 
 * ionospheric  correction  algorithm  NeQuickG to  estimate  the ionospheric delay on each satellite link.<br>
 * 
 * The implementation has been written in the C programming language standard 2011 and is divided in:
 *  - The NeQuickG JRC library (src/lib) and
 *  - The test driver program (src)
 *
 * <h3>Spec(s):</h3>
 *  - European GNSS (Galileo) Open Service. Ionospheric Correction Algorithm for Galileo Single Frequency Users, 1.2 September 2016
 *  - C standard ISO/IEC 9899:2011
 *
 * <h3>Requirements</h3>
 *  - One of the toolchains listed in Configuration
 *  - perl for test execution
 *  - Doxygen, a documentation generator
 *  - genhtml, a coverage HTML report generator
 *  - lcov, a coverage tool
 *
 * <h3>Configuration</h3>
 * Three toolchains are supported:
 *  - GNU compiler gcc
 *  - LLVM Project clang
 *  - Microsoft Visual Studio 2015
 *
 * <h4>GCC, Clang</h4>
 *   open terminal, navigate to build/gcc or build/clang <br>
 *   list of targets:
 *    - check ==> runs the system tests, if debug runs the unit tests too.
 *    - clean ==> clean release target
 *    - clean_all ==> clean everything
 *    - debug ==> builds the debug target
 *    - doxygen ==> creates the doxygen documentation
 *    - help ==> outputs this list of targets
 *    - lint ==> runs the lint static analyzer
 *    - release ==> builds the release target
 *
 *  The debug version is built with the following functionalities:
 *    - unit tests (level of software testing where individual units/ components of a software are tested.)
 *    - GCC AddressSanitizer (Memory access instructions are instrumented to detect out-of-bounds and use-after-free bugs.)
 *    - profile information suitable for the analysis program gprof.
 *
 * @note On Cygwin the AddressSanitizer is not available.
 * @note Only the lint version for Windows is supported .
 *
 * <h5>Compilation options</h5>
 * FTR_MODIP_CCIR_AS_CONSTANTS=1 ==> CCIR grid and the MODIP files are preloaded as constants in the library
 *
 * <h5>Examples</h5>
 *  - make release check builds the release target and executes the system tests
 *  - make debug check builds the debug target and executes the system tests plus the unit tests and generates the coverage information
 *  - make release FTR_MODIP_CCIR_AS_CONSTANTS=1 builds the release target with preloaded MODIP grid and CCIR files.
 *
 * <h4>Microsoft Visual Studio</h4>
 *  open project using IDE at msvc/NeQuickJRC<br>
 *  there are targets available for debug, Unit test, x64, x86 and MODIP/CCIRs as preloaded constants<br>
 *  select Release target in the configuration management to get the release version<br>
 *  to test the target open a command line at msvc and run "make check".
 *
 * <h3>Usage of the test driver program</h3>
 *  - open terminal at bin/(selected_compiler)
 *  - run ./NeQuickG_JRC.exe -h for a list of options.
 *
 * <h3>CHANGELOG</h3>
 *   First version.
 *
 * <h3>Acknowlegements</h3>
 * The NeQuick electron density model was developed by the Abdus Salam International Center of Theoretical
 * Physics (ICTP) and the University of Graz. The adaptation of NeQuick for the Galileo single-frequency ionospheric
 * correction algorithm (NeQuick G) has been performed by the European Space Agency (ESA) involving the original
 * authors and other European ionospheric scientists under various ESA contracts. The step-by-step algorithmic
 * description of NeQuick for Galileo has been a collaborative effort of IC TP, ESA and the European Commission, including JRC.
  */
namespace README { }
