/**
 * @file LCD.h
 * @brief Prototipos de funciones y macros para el control de un LCD mediante I2C.
 * 
 * Este archivo define las macros necesarias para la configuración del LCD (dirección, comandos,
 * pines I2C, etc.) y declara los prototipos de funciones para inicializar y controlar el display.
 * 
 * Autores: Angel, Benjamin, Kevin
 * Fecha: [Especifica la fecha]
 */

 #ifndef _LCD_H
 #define _LCD_H
 
 #include <stdbool.h>  /**< Incluye la definición del tipo bool */
 #include <stdint.h>   /**< Incluye la definición del tipo uint8_t */
 
 /** @def I2C_PORT
  *  @brief Puerto I2C utilizado para la comunicación con el LCD.
  */
 #define I2C_PORT i2c0
 
 /** @def LCD_ADDR
  *  @brief Dirección I2C del LCD.
  */
 #define LCD_ADDR 0x27
 
 /** @def LCD_ROWS
  *  @brief Número de filas del display LCD.
  */
 #define LCD_ROWS 2
 
 /** @def LCD_COLS
  *  @brief Número de columnas del display LCD.
  */
 #define LCD_COLS 16
 
 /** @def SDA_PIN
  *  @brief Pin SDA para la comunicación I2C en la Raspberry Pi Pico.
  */
 #define SDA_PIN 0
 
 /** @def SCL_PIN
  *  @brief Pin SCL para la comunicación I2C en la Raspberry Pi Pico.
  */
 #define SCL_PIN 1
 
 // Comandos para el LCD
 
 /** @def LCD_CLEARDISPLAY
  *  @brief Comando para limpiar el display.
  */
 #define LCD_CLEARDISPLAY 0x01
 
 /** @def LCD_RETURNHOME
  *  @brief Comando para retornar el cursor a la posición de inicio.
  */
 #define LCD_RETURNHOME 0x02
 
 /** @def LCD_ENTRYMODESET
  *  @brief Comando para configurar el modo de entrada del LCD.
  */
 #define LCD_ENTRYMODESET 0x04
 
 /** @def LCD_DISPLAYCONTROL
  *  @brief Comando para controlar el display (encendido/apagado, cursor, etc.).
  */
 #define LCD_DISPLAYCONTROL 0x08
 
 /** @def LCD_CURSORSHIFT
  *  @brief Comando para desplazar el cursor o el display.
  */
 #define LCD_CURSORSHIFT 0x10
 
 /** @def LCD_FUNCTIONSET
  *  @brief Comando para configurar la función del LCD (modo de datos, número de líneas, etc.).
  */
 #define LCD_FUNCTIONSET 0x20
 
 /** @def LCD_SETCGRAMADDR
  *  @brief Comando para establecer la dirección de la memoria CGRAM.
  */
 #define LCD_SETCGRAMADDR 0x40
 
 /** @def LCD_SETDDRAMADDR
  *  @brief Comando para establecer la dirección de la memoria DDRAM.
  */
 #define LCD_SETDDRAMADDR 0x80
 
 // Flags para el modo de entrada del display
 
 /** @def LCD_ENTRYLEFT
  *  @brief Flag para configurar la escritura de izquierda a derecha.
  */
 #define LCD_ENTRYLEFT 0x02
 
 // Flags para el control del display y el cursor
 
 /** @def LCD_DISPLAYON
  *  @brief Flag para encender el display.
  */
 #define LCD_DISPLAYON 0x04
 
 // Flags para la función set
 
 /** @def LCD_2LINE
  *  @brief Flag para configurar el display en dos líneas.
  */
 #define LCD_2LINE 0x08
 
 /** @def LCD_4BITMODE
  *  @brief Flag para configurar el LCD en modo de 4 bits.
  */
 #define LCD_4BITMODE 0x00
 
 // Control de la retroiluminación
 
 /** @def LCD_BACKLIGHT
  *  @brief Valor para activar la retroiluminación del LCD.
  */
 #define LCD_BACKLIGHT 0x08
 
 /**
  * @brief Envía un byte a través de I2C al LCD.
  *
  * Esta función escribe un byte en el LCD utilizando la interfaz I2C.
  *
  * @param val Byte a enviar.
  */
 void i2c_write_byte(uint8_t val);
 
 /**
  * @brief Alterna la señal de habilitación del LCD.
  *
  * Esta función realiza un pulso en el pin de enable del LCD para que el display
  * procese los datos o comandos enviados.
  *
  * @param val Valor actual del byte que se enviará (utilizado para establecer el Enable).
  */
 void lcd_toggle_enable(uint8_t val);
 
 /**
  * @brief Envía un byte al LCD dividiéndolo en dos nibbles.
  *
  * Se envía el byte en dos partes (nibbles alto y bajo) para trabajar en modo de 4 bits.
  *
  * @param val Byte a enviar.
  * @param mode Modo: 0 para comando, 1 para dato.
  */
 void lcd_send_byte(uint8_t val, int mode);
 
 /**
  * @brief Envía un comando al LCD.
  *
  * Esta función envía un comando al display LCD para controlar su funcionamiento.
  *
  * @param command Comando a enviar.
  */
 void lcd_send_command(uint8_t command);
 
 /**
  * @brief Envía datos al LCD.
  *
  * Envía un valor de datos (por ejemplo, un carácter) al display LCD.
  *
  * @param data_value Valor de datos a enviar.
  */
 void lcd_send_data(uint8_t data_value);
 
 /**
  * @brief Controla la retroiluminación del LCD.
  *
  * Activa o desactiva la retroiluminación del display.
  *
  * @param state true para encender la retroiluminación, false para apagarla.
  */
 void lcd_backlight(bool state);
 
 /**
  * @brief Inicializa el display LCD.
  *
  * Configura el LCD en modo de 4 bits, inicializa los parámetros y lo prepara para mostrar datos.
  */
 void lcd_init();
 
 /**
  * @brief Establece la posición del cursor en el LCD.
  *
  * Posiciona el cursor en la fila y columna especificadas.
  *
  * @param row Fila en la que se ubicará el cursor.
  * @param col Columna en la que se ubicará el cursor.
  */
 void lcd_set_cursor(uint8_t row, uint8_t col);
 
 /**
  * @brief Imprime una cadena de caracteres en el LCD.
  *
  * Envía una cadena de texto para que se muestre en el display.
  *
  * @param str Cadena de caracteres a imprimir.
  */
 void lcd_print(const char *str);
 
 /**
  * @brief Limpia el contenido del display LCD.
  *
  * Envía el comando para borrar el contenido del LCD.
  */
 void lcd_clear();
 
 /**
  * @brief Inicializa los pines I2C para la comunicación con el LCD.
  *
  * Configura los pines SDA y SCL de la Raspberry Pi Pico para que funcionen en modo I2C.
  */
 void init_gpios();
 
 #endif /* _LCD_H */
 