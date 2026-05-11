#include "lift.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lift, LOG_LEVEL_INF);

static const struct gpio_dt_spec motor_output_a = GPIO_DT_SPEC_GET(DT_ALIAS(motor_output_a), gpios);
static const struct gpio_dt_spec motor_output_b = GPIO_DT_SPEC_GET(DT_ALIAS(motor_output_b), gpios);

int lift_init()
{
	int ret;
    ret = gpio_is_ready_dt(&motor_output_a);
	if (ret < 0)
    {
		return ret;
	}

    ret = gpio_is_ready_dt(&motor_output_b);
	if (!ret < 0)
    {
		return ret;
	}

	ret = gpio_pin_configure_dt(&motor_output_a, GPIO_OUTPUT_INACTIVE);
	if (ret < 0)
    {
		return ret;
	}

	ret = gpio_pin_configure_dt(&motor_output_b, GPIO_OUTPUT_INACTIVE);
	if (ret < 0)
    {
		return ret;
	}
    return 0;
}

int lift_rise()
{
	int ret = 0;
	ret = gpio_pin_set_dt(&motor_output_a, 1);
	if (ret < 0) {
		return ret;
	}
	ret = gpio_pin_set_dt(&motor_output_b, 0);
	if (ret < 0) {
		return ret;
	}
	LOG_INF("Lift rising\n");
	return 0;
}

int lift_lower()
{
	int ret = 0;
	gpio_pin_set_dt(&motor_output_a, 0);
	if (ret < 0) {
		return ret;
	}
	gpio_pin_set_dt(&motor_output_b, 1);
	if (ret < 0) {
		return ret;
	}
	LOG_INF("Lift lowering\n");
	return 0;
}

int lift_stop()
{
	int ret = 0;
	ret = gpio_pin_set_dt(&motor_output_a, 0);
	if (ret < 0) {
		return ret;
	}
	ret = gpio_pin_set_dt(&motor_output_b, 0);
	if (ret < 0) {
		return ret;
	}
	LOG_INF("Lift stopped\n");
	return 0;
}


