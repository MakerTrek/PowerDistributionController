#include "joystick.h"
#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>
#include <zephyr/input/input.h>

LOG_MODULE_REGISTER(joystick, LOG_LEVEL_INF);

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif


/* Data of ADC io-channels specified in devicetree. */
// TODO: this FOREACH doesn't wokr as expected for some reason.
// not nessesary for this codebase since the joystick is expected to have exactly 2 axis.
// static const struct adc_dt_spec adc_channels[] = {
// 	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA_FOR_INPUTS)
// };
static const struct adc_dt_spec adc_channels[] = {
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
};

uint32_t buf = 0;
struct adc_sequence sequence = {
    .buffer = &buf,
    /* buffer size in bytes, not number of samples */
    .buffer_size = sizeof(buf),
#if CONFIG_SAMPLE_ADC_CALIBRATE_REQUIRED
		.calibrate = true,
#endif
};

void (*_joystick_pressed_cb)();
void (*_joystick_released_cb)();

// 
static void joystick_input_keys_cb(struct input_event *evt, void *user_data)
{
    ARG_UNUSED(user_data);

    if (evt->code == INPUT_KEY_ENTER) {
        if (evt->value)
        {
            _joystick_pressed_cb();
        }
        else
        {
            _joystick_released_cb();
        }
    }
}

INPUT_CALLBACK_DEFINE(NULL, joystick_input_keys_cb, NULL);

// returns 0 on success, 
int joystick_init(void (*joystick_pressed_cb)(void), void (*joystick_released_cb)(void))
{
    _joystick_pressed_cb = joystick_pressed_cb;
    _joystick_released_cb = joystick_released_cb;

    int err = 0;
    
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
    {
        err = adc_is_ready_dt(&adc_channels[i]);
		if (err)
        {
			LOG_ERR("ADC controller device %s not working\n", adc_channels[i].dev->name);
            return err;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0)
        {
			LOG_ERR("Could not setup channel #%d (%d)\n", i, err);
            return err;
		}
	}
	return 0;
}

int joystick_get_position(int16_t *x, int16_t *y)
{
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
    {
        int32_t val_mv = 0;
        int err = 0;
    
        LOG_INF("- %s, channel %d: ", adc_channels[i].dev->name, adc_channels[i].channel_id);

        (void)adc_sequence_init_dt(&adc_channels[i], &sequence);

        err = adc_read_dt(&adc_channels[i], &sequence);
        if (err < 0) {
            LOG_ERR("Could not read (%d)", err);
            continue;
        }

        if (adc_channels[i].channel_cfg.differential) {
            val_mv = (int32_t)((int16_t)buf);
        } else {
            val_mv = (int32_t)buf;
        }
        
        LOG_INF("%"PRId32, val_mv);

        err = adc_raw_to_millivolts_dt(&adc_channels[i], &val_mv);
        /* conversion to mV may not be supported, skip if not */
        if (err < 0) {
            LOG_ERR(" (value in mV not available)");
        } else {
            LOG_INF(" = %"PRId32" mV", val_mv);
        }

        if(i == 0) *x = val_mv;
        else if(i == 1) *y = val_mv;
        else return 0;
    }
    return 0;
}