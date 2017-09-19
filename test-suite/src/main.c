#include <stdlib.h>
#include <stdio.h>

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "types.h"
#include "constants.h"

#include "sensor.h"

/* --------------------------------------------------------------------------------
 *                              FSM Conformance Test
 * --------------------------------------------------------------------------------
 */

// State Machine expected behavior container structure
typedef struct
{
    sensor_echo_state_t state_id;       // Current state id
    sensor_echo_state_t next_state[4];  // Expected next state, based on the input
    bool_t              next_output[4]; // Expected next output, based on the input
} sm_state_t;

// Actual expected behavior, see enum declarations in sensor.h
// file to see what these numbers are mapped into
const sm_state_t sm[] =
{
    {0,{3,3,2,1},{0,0,1,1}},
    {1,{3,3,2,1},{0,0,1,1}},
    {2,{3,3,2,1},{0,0,1,1}},
    {3,{3,3,0,0},{0,0,1,1}},
};

// Tests if a given state implementation is good or wrong
void conformance_test_state(const sensor_echo_state_t state_id)
{
    sensor_t sensor;

    // REC_TRIG

    // Move to starting state
    sensor.echo_state = state_id;
    // Update function
    check_finished(&sensor, IN_TO_RECORDING(REC_TRIG), IN_TO_TRIGGER_SENT(REC_TRIG));
    // Moved to correct state
    CU_ASSERT_EQUAL(sensor.echo_state, sm[state_id].next_state[REC_TRIG]);
    // Obtained correct output
    CU_ASSERT_EQUAL(sensor.trigger_sent, sm[state_id].next_output[REC_TRIG]);

    // REC_NTRIG

    // Move to starting state
    sensor.echo_state = state_id;
    // Update function
    check_finished(&sensor, IN_TO_RECORDING(REC_NTRIG), IN_TO_TRIGGER_SENT(REC_NTRIG));
    // Moved to correct state
    CU_ASSERT_EQUAL(sensor.echo_state, sm[state_id].next_state[REC_NTRIG]);
    // Obtained correct output
    CU_ASSERT_EQUAL(sensor.trigger_sent, sm[state_id].next_output[REC_NTRIG]);

    // NREC_TRIG

    // Move to starting state
    sensor.echo_state = state_id;
    // Update function
    check_finished(&sensor, IN_TO_RECORDING(NREC_TRIG), IN_TO_TRIGGER_SENT(NREC_TRIG));
    // Moved to correct state
    CU_ASSERT_EQUAL(sensor.echo_state, sm[state_id].next_state[NREC_TRIG]);
    // Obtained correct output
    CU_ASSERT_EQUAL(sensor.trigger_sent, sm[state_id].next_output[NREC_TRIG]);

    // NREC_NTRIG

    // Move to starting state
    sensor.echo_state = state_id;
    // Update function
    check_finished(&sensor, IN_TO_RECORDING(NREC_NTRIG), IN_TO_TRIGGER_SENT(NREC_NTRIG));
    // Moved to correct state
    CU_ASSERT_EQUAL(sensor.echo_state, sm[state_id].next_state[NREC_NTRIG]);
    // Obtained correct output
    CU_ASSERT_EQUAL(sensor.trigger_sent, sm[state_id].next_output[NREC_NTRIG]);
}

void conformance_test_state_nok()
{
    conformance_test_state(SENSOR_ECHO_NEXT_OK);
}

void conformance_test_state_ok()
{
    conformance_test_state(SENSOR_ECHO_OK);
}

void conformance_test_state_lost()
{
    conformance_test_state(SENSOR_ECHO_LOST);
}

void conformance_test_state_long()
{
    conformance_test_state(SENSOR_ECHO_LONG);
}


/* --------------------------------------------------------------------------------
 *                        Triangolation Functional Testing
 * --------------------------------------------------------------------------------
 */

#define LX_MIN  0
#define LX_MAX  SENSOR_DIST_MAX
#define RX_MIN  0
#define RX_MAX  SENSOR_DIST_MAX

#define LX_NOMINAL ((LX_MIN + LX_MAX) / 2)
#define RX_NOMINAL ((RX_MIN + RX_MAX) / 2)

#define LX_EPSILON  ((LX_MAX - LX_MIN) / 20)
#define RX_EPSILON  ((RX_MAX - RX_MIN) / 20)

#define LX_BOUNDARY_MIN (LX_MIN + LX_EPSILON)
#define LX_BOUNDARY_MAX (LX_MAX - LX_EPSILON)

#define RX_BOUNDARY_MIN (RX_MIN + RX_EPSILON)
#define RX_BOUNDARY_MAX (RX_MAX - RX_EPSILON)

#define LX_ROBUSTNESS_MIN (LX_MIN - LX_EPSILON)
#define LX_ROBUSTNESS_MAX (LX_MAX + LX_EPSILON)

#define RX_ROBUSTNESS_MIN (RX_MIN - RX_EPSILON)
#define RX_ROBUSTNESS_MAX (RX_MAX + RX_EPSILON)

/* ---------------------------------- Original Version ------------------------------------ */

void triangolation_nominal()
{
    CU_ASSERT_EQUAL(triangolation_original(LX_NOMINAL, RX_NOMINAL), LX_NOMINAL);
}

void triangolation_boundary()
{
    // Testing LX boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_MAX, RX_NOMINAL), RX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_original(LX_MIN, RX_NOMINAL), LX_MIN);

    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MAX, RX_NOMINAL), RX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MIN, RX_NOMINAL), LX_BOUNDARY_MIN);

    // Testing RX boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_NOMINAL, RX_MAX), LX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_original(LX_NOMINAL, RX_MIN), RX_MIN);

    CU_ASSERT_EQUAL(triangolation_original(LX_NOMINAL, RX_BOUNDARY_MAX), LX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_original(LX_NOMINAL, RX_BOUNDARY_MIN), RX_BOUNDARY_MIN);
}

void triangolation_robustness()
{
    // Testing LX robustness
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MAX, RX_NOMINAL), RX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MIN, RX_NOMINAL), LX_MIN);

    // Testing RX robustness
    CU_ASSERT_EQUAL(triangolation_original(LX_NOMINAL, RX_ROBUSTNESS_MAX), LX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_original(LX_NOMINAL, RX_ROBUSTNESS_MIN), RX_MIN);
}

void triangolation_worstcase()
{
    // Testing LX max boundary - RX max boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_MAX,          RX_MAX),            RX_MAX);
    CU_ASSERT_EQUAL(triangolation_original(LX_MAX,          RX_BOUNDARY_MAX),   RX_BOUNDARY_MAX);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MAX, RX_MAX),            LX_BOUNDARY_MAX);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MAX, RX_BOUNDARY_MAX),   LX_BOUNDARY_MAX);

    // Testing LX max boundary - RX min boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_MAX,          RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_MAX,          RX_BOUNDARY_MIN),   RX_BOUNDARY_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MAX, RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MAX, RX_BOUNDARY_MIN),   RX_BOUNDARY_MIN);

    // Testing LX min boundary - RX max boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_MIN,          RX_MAX),            LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_MIN,          RX_BOUNDARY_MAX),   LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MIN, RX_MAX),            LX_BOUNDARY_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MIN, RX_BOUNDARY_MAX),   LX_BOUNDARY_MIN);

    // Testing LX min boundary - RX min boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_MIN,          RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_MIN,          RX_BOUNDARY_MIN),   LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MIN, RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MIN, RX_BOUNDARY_MIN),   LX_BOUNDARY_MIN);
}

void triangolation_wc_robustness()
{
    // Testing LX max boundary - RX max boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MAX,   RX_ROBUSTNESS_MAX), RX_MAX);
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MAX,   RX_MAX),            LX_MAX);
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MAX,   RX_BOUNDARY_MAX),   RX_BOUNDARY_MAX);
    CU_ASSERT_EQUAL(triangolation_original(LX_MAX,              RX_ROBUSTNESS_MAX), LX_MAX);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MAX,     RX_ROBUSTNESS_MAX), LX_BOUNDARY_MAX);
    
    // Testing LX max boundary - RX min boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MAX,   RX_ROBUSTNESS_MIN), RX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MAX,   RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MAX,   RX_BOUNDARY_MIN),   RX_BOUNDARY_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_MAX,              RX_ROBUSTNESS_MIN), RX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MAX,     RX_ROBUSTNESS_MIN), RX_MIN);

    // Testing LX min boundary - RX max boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MIN,   RX_ROBUSTNESS_MAX), LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MIN,   RX_MAX),            LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MIN,   RX_BOUNDARY_MAX),   LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_MIN,              RX_ROBUSTNESS_MAX), LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MIN,     RX_ROBUSTNESS_MAX), LX_BOUNDARY_MIN);

    // Testing LX min boundary - RX min boundary
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MIN,   RX_ROBUSTNESS_MIN), LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MIN,   RX_MIN),            LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_ROBUSTNESS_MIN,   RX_BOUNDARY_MIN),   LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_MIN,              RX_ROBUSTNESS_MIN), LX_MIN);
    CU_ASSERT_EQUAL(triangolation_original(LX_BOUNDARY_MIN,     RX_ROBUSTNESS_MIN), RX_MIN);
}

void triangolation_random()
{
    CU_ASSERT_EQUAL(triangolation_original(LX_NOMINAL-1, RX_NOMINAL+1), (RX_NOMINAL+LX_NOMINAL) / 2);
    CU_ASSERT_EQUAL(triangolation_original(LX_NOMINAL+1, RX_NOMINAL-1), (RX_NOMINAL+LX_NOMINAL) / 2);
}

/* ---------------------------------- Fixed Version ------------------------------------ */

void triangolation_nominal_fix()
{
    CU_ASSERT_EQUAL(triangolation_fixed(LX_NOMINAL, RX_NOMINAL), LX_NOMINAL);
}

void triangolation_boundary_fix()
{
    // Testing LX boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MAX, RX_NOMINAL), RX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MIN, RX_NOMINAL), LX_MIN);

    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MAX, RX_NOMINAL), RX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MIN, RX_NOMINAL), LX_BOUNDARY_MIN);

    // Testing RX boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_NOMINAL, RX_MAX), LX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_NOMINAL, RX_MIN), RX_MIN);

    CU_ASSERT_EQUAL(triangolation_fixed(LX_NOMINAL, RX_BOUNDARY_MAX), LX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_NOMINAL, RX_BOUNDARY_MIN), RX_BOUNDARY_MIN);
}

void triangolation_robustness_fix()
{
    // Testing LX robustness
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MAX, RX_NOMINAL), RX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MIN, RX_NOMINAL), LX_MIN);

    // Testing RX robustness
    CU_ASSERT_EQUAL(triangolation_fixed(LX_NOMINAL, RX_ROBUSTNESS_MAX), LX_NOMINAL);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_NOMINAL, RX_ROBUSTNESS_MIN), RX_MIN);
}

void triangolation_worstcase_fix()
{
    // Testing LX max boundary - RX max boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MAX,          RX_MAX),            RX_MAX);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MAX,          RX_BOUNDARY_MAX),   RX_BOUNDARY_MAX);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MAX, RX_MAX),            LX_BOUNDARY_MAX);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MAX, RX_BOUNDARY_MAX),   LX_BOUNDARY_MAX);

    // Testing LX max boundary - RX min boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MAX,          RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MAX,          RX_BOUNDARY_MIN),   RX_BOUNDARY_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MAX, RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MAX, RX_BOUNDARY_MIN),   RX_BOUNDARY_MIN);

    // Testing LX min boundary - RX max boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MIN,          RX_MAX),            LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MIN,          RX_BOUNDARY_MAX),   LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MIN, RX_MAX),            LX_BOUNDARY_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MIN, RX_BOUNDARY_MAX),   LX_BOUNDARY_MIN);

    // Testing LX min boundary - RX min boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MIN,          RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MIN,          RX_BOUNDARY_MIN),   LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MIN, RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MIN, RX_BOUNDARY_MIN),   LX_BOUNDARY_MIN);
}

void triangolation_wc_robustness_fix()
{
    // Testing LX max boundary - RX max boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MAX,   RX_ROBUSTNESS_MAX), RX_MAX);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MAX,   RX_MAX),            LX_MAX);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MAX,   RX_BOUNDARY_MAX),   RX_BOUNDARY_MAX);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MAX,              RX_ROBUSTNESS_MAX), LX_MAX);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MAX,     RX_ROBUSTNESS_MAX), LX_BOUNDARY_MAX);
    
    // Testing LX max boundary - RX min boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MAX,   RX_ROBUSTNESS_MIN), RX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MAX,   RX_MIN),            RX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MAX,   RX_BOUNDARY_MIN),   RX_BOUNDARY_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MAX,              RX_ROBUSTNESS_MIN), RX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MAX,     RX_ROBUSTNESS_MIN), RX_MIN);

    // Testing LX min boundary - RX max boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MIN,   RX_ROBUSTNESS_MAX), LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MIN,   RX_MAX),            LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MIN,   RX_BOUNDARY_MAX),   LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MIN,              RX_ROBUSTNESS_MAX), LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MIN,     RX_ROBUSTNESS_MAX), LX_BOUNDARY_MIN);

    // Testing LX min boundary - RX min boundary
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MIN,   RX_ROBUSTNESS_MIN), LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MIN,   RX_MIN),            LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_ROBUSTNESS_MIN,   RX_BOUNDARY_MIN),   LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_MIN,              RX_ROBUSTNESS_MIN), LX_MIN);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_BOUNDARY_MIN,     RX_ROBUSTNESS_MIN), RX_MIN);
}

void triangolation_random_fix()
{
    CU_ASSERT_EQUAL(triangolation_fixed(LX_NOMINAL-1, RX_NOMINAL+1), (RX_NOMINAL+LX_NOMINAL) / 2);
    CU_ASSERT_EQUAL(triangolation_fixed(LX_NOMINAL+1, RX_NOMINAL-1), (RX_NOMINAL+LX_NOMINAL) / 2);
}





/* --------------------------------------------------------------------------------
 *                                    Main
 * --------------------------------------------------------------------------------
 */

int main(int argc, char* argv[])
{
    if(CUE_SUCCESS != CU_initialize_registry())
    {
        printf("Could not initialize CUnit registry!\r\n");
        
        return CU_get_error();
    }

    CU_set_error_action(CUEA_FAIL);    

    // Test on the FSM

    CU_pSuite conformance = CU_add_suite("FSM Conformance Test", NULL, NULL);

    CU_add_test(conformance, "State NEXT_OK Conformance Test", conformance_test_state_nok);
    CU_add_test(conformance, "State OK      Conformance Test", conformance_test_state_ok);
    CU_add_test(conformance, "State LOST    Conformance Test", conformance_test_state_lost);
    CU_add_test(conformance, "State LONG    Conformance Test", conformance_test_state_long);
    
    // Test on the triangolation
    
    CU_pSuite triangolation = CU_add_suite("Original Triangolation Functional Testing", NULL, NULL);
    
    CU_add_test(triangolation, "Nominal Testing", triangolation_nominal);
    CU_add_test(triangolation, "Boundary Testing", triangolation_boundary);
    CU_add_test(triangolation, "Robustness Testing", triangolation_robustness);
    CU_add_test(triangolation, "Worst-Case Testing", triangolation_worstcase);
    CU_add_test(triangolation, "Worst-Case Robustness Testing", triangolation_wc_robustness);
    CU_add_test(triangolation, "Random Testing", triangolation_random);

    CU_pSuite triangolation_fix = CU_add_suite("Fixed Triangolation Functional Testing", NULL, NULL);

    CU_add_test(triangolation_fix, "Nominal Testing", triangolation_nominal_fix);
    CU_add_test(triangolation_fix, "Boundary Testing", triangolation_boundary_fix);
    CU_add_test(triangolation_fix, "Robustness Testing", triangolation_robustness_fix);
    CU_add_test(triangolation_fix, "Worst-Case Testing", triangolation_worstcase_fix);
    CU_add_test(triangolation_fix, "Worst-Case Robustness Testing", triangolation_wc_robustness_fix);
    CU_add_test(triangolation_fix, "Random Testing", triangolation_random_fix);

    
    // Test on the motor

    // TODO:


    // Running all the tests
    CU_set_output_filename("res-unit/Sonar-Test");
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_cleanup_registry();

    return EXIT_SUCCESS;
}