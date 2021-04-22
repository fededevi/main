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


void app_main(void)
{
    TFT_fillScreen(TFT_BLUE);
    //TFT_resetclipwin();

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
