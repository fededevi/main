/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"


#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "pingo/math/vec2.h"
#include "pingo/render/mesh.h"
#include "pingo/example/teapot.h"
#include "pingo/example/cube.h"
#include "pingo/example/memorybackend.h"
#include "pingo/render/renderer.h"
#include "pingo/render/texture.h"
#include "pingo/render/sprite.h"
#include "pingo/render/scene.h"
#include "pingo/render/object.h"
#include "pingo/math/mat3.h"

#include "tft_espi/tft.h"

#define SPI_BUS TFT_HSPI_HOST

spi_lobo_device_handle_t spi;
void app_main(void)
{
    tft_disp_type = DISP_TYPE_ST7789V;

    max_rdclock = 8000000;

    printf("TFT_PinsInit");
    TFT_PinsInit();

    spi_lobo_bus_config_t buscfg={
        .miso_io_num=PIN_NUM_MISO,				// set SPI MISO pin
        .mosi_io_num=PIN_NUM_MOSI,				// set SPI MOSI pin
        .sclk_io_num=PIN_NUM_CLK,				// set SPI CLK pin
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz = 6*1024,
    };

    spi_lobo_device_interface_config_t devcfg={
        .clock_speed_hz=8000000,                // Initial clock out at 8 MHz
        .mode=0,                                // SPI mode 0
        .spics_io_num=-1,                       // we will use external CS pin
        .spics_ext_io_num=PIN_NUM_CS,           // external CS pin
        .flags=LB_SPI_DEVICE_HALFDUPLEX,        // ALWAYS SET  to HALF DUPLEX MODE!! for display spi
    };

    vTaskDelay(500 / portTICK_RATE_MS);
    printf("\r\n==============================\r\n");
    printf("TFT display DEMO, LoBo 11/2017\r\n");
    printf("==============================\r\n");
    printf("Pins used: miso=%d, mosi=%d, sck=%d, cs=%d\r\n", PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_CLK, PIN_NUM_CS);


    printf("==============================\r\n\r\n");

    // ==================================================================
    // ==== Initialize the SPI bus and attach the LCD to the SPI bus ====

    esp_err_t ret=spi_lobo_bus_add_device(SPI_BUS, &buscfg, &devcfg, &spi);
    assert(ret==ESP_OK);
    printf("SPI: display device added to spi bus (%d)\r\n", SPI_BUS);
    disp_spi = spi;

    // ==== Test select/deselect ====
    ret = spi_lobo_device_select(spi, 1);
    assert(ret==ESP_OK);
    ret = spi_lobo_device_deselect(spi);
    assert(ret==ESP_OK);

    printf("SPI: attached display device, speed=%u\r\n", spi_lobo_get_speed(spi));
    printf("SPI: bus uses native pins: %s\r\n", spi_lobo_uses_native_pins(spi) ? "true" : "false");


    printf("SPI: display init...\r\n");
    TFT_display_init();
    printf("OK\r\n");

    // ---- Detect maximum read speed ----
    max_rdclock = find_rd_speed();
    printf("SPI: Max rd speed = %u\r\n", max_rdclock);

    // ==== Set SPI clock used for display operations ====
    spi_lobo_set_speed(spi, DEFAULT_SPI_CLOCK);
    printf("SPI: Changed speed to %u\r\n", spi_lobo_get_speed(spi));

    printf("\r\n---------------------\r\n");
    printf("Graphics demo started\r\n");
    printf("---------------------\r\n");

    font_rotate = 0;
    text_wrap = 0;
    font_transparent = 0;
    font_forceFixed = 0;
    gray_scale = 0;
    TFT_setGammaCurve(DEFAULT_GAMMA_CURVE);
    TFT_setRotation(PORTRAIT);
    TFT_fillScreen(TFT_BLUE);
    TFT_resetclipwin();

    while (1) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    /*
    Vec2i size = {135, 135};

    MemoryBackend mB;
    memoryBackendInit(&mB, size);

    Renderer renderer;
    rendererInit(&renderer, size,(BackEnd*) &mB );

    Scene s;
    sceneInit(&s);
    rendererSetScene(&renderer, &s);

    Object cube1;
    cube1.mesh = &mesh_cube;
    sceneAddRenderable(&s, object_as_renderable(&cube1));
    cube1.material = 0;

    Object cube2;
    cube2.mesh = &mesh_cube;
    sceneAddRenderable(&s, object_as_renderable(&cube2));

    //TEXTURE FOR CUBE 2
    Texture tex;
    texture_init(&tex, (Vec2i){8,8}, malloc(8*8*sizeof(Pixel)));

    for (int i = 0; i < 8; i++)
        for (int y = 0; y < 8; y++)
            ((uint32_t *)tex.frameBuffer)[i * 8 + y ] = (i + y) % 2 == 0 ? 0xFFFFFFFF : 0x000000FF;

    Material m;
    m.texture = &tex;
    cube2.material = &m;

    Object tea;
    tea.mesh = &mesh_teapot;
    sceneAddRenderable(&s, object_as_renderable(&tea));
    tea.material = 0;

    float phi = 0;
    float phi2 = 0;
    Mat4 t;

    while (1) {
        renderer.camera_projection = mat4Perspective( 2, 16.0,(float)size.x / (float)size.y, 50.0);

        //VIEW MATRIX
        Mat4 v = mat4Translate((Vec3f) { 0,0,-9});
        Mat4 rotateDown = mat4RotateX(0.40); //Rotate around origin/orbit
        renderer.camera_view = mat4MultiplyM(&rotateDown, &v );

        //CUBE 1 TRANSFORM
        cube1.transform =  mat4RotateY(phi2 -= 0.01);
        t = mat4Scale((Vec3f){1,1,1});
        cube1.transform = mat4MultiplyM(&cube1.transform, &t );
        t = mat4Translate((Vec3f){-5,0.0,0});
        cube1.transform = mat4MultiplyM(&cube1.transform, &t );

        //CUBE 2 TRANSFORM
        cube2.transform =  mat4Translate((Vec3f){5,0.0,0});
        t = mat4Scale((Vec3f){1,1,1});
        cube2.transform = mat4MultiplyM(&cube2.transform, &t );


        //TEA TRANSFORM
        tea.transform = mat4RotateZ(PI);
        t =mat4RotateY(phi2);
        tea.transform = mat4MultiplyM(&tea.transform, &t );
        t = mat4Translate((Vec3f){0,-0.5,0});
        tea.transform = mat4MultiplyM(&tea.transform, &t );

        //SCENE
        s.transform = mat4RotateY(phi += 0.01);

        rendererSetCamera(&renderer,(Vec4i){0,0,size.x,size.y});
        printf("1");
        rendererRender(&renderer);

        printf("PHY %f\n", phi);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
*/

    /*
    gpio_pad_select_gpio(BLINK_GPIO);

    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {

        printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(10 / portTICK_PERIOD_MS);

        printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }*/
}
