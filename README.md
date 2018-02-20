## 这是一个使用STM32单片机实现的电压电流采集，AD转换并且将采集的数据进行快速傅里叶变换生成频谱图的示例代码
This is a demo using STM32F103z to convert voltages' A/D and transform basic data into spectrum using FFT algorithm


### 需要移植的移植库文件：

- __table_fft.h__ (FFT表文件)
- __stm32_dsp.h__ (调用DSP库函数的头文件，如果是自己编写的fft，则不需要此文件)
- __cr4fft64_stm32.s__ 和 __cr4fft256_stm32.s__ 和 __cr4fft1024_stm32.s__ （DSP傅里叶变换的可执行函数，如果是自己完成的fft则不需要此文件）
- __FFT2.c__（和同一目录下FFT2)  这个是自己实现FFT的函数所在文件，其中包含简单的数据处理，滤波（如果是调用dps函数完成，则不需要此文件）
- __dsp_g2.c__ (测试序列发生函数，fft调用函数，串口通信函数，全部封装在一个函数内，在main函数里面调用即可，这个是自己实现的FFT)
- __dsp_asm.c__ （同上，不过这个是直接调用库函数的FFT，不推荐使用）


### Essential library files：

- __table_fft.h__ (Fast Fourier Transform Table Files)
- __stm32_dsp.h__ (Used to call the DSP library function header files, you do not need this file if you have prepared the Fast Fourier Transform using software algorithm already)
- __cr4fft64_stm32.s__ and __cr4fft256_stm32.s__ and __cr4fft1024_stm32.s__ （Executable functions used to perform DSP's Fourier Transform, you do not need this file if you have prepared the Fast Fourier Transform using software algorithm already）
- __FFT2.c__（And the FFT2 library files under the same directory) This is the function of their own FFT file, which contains a simple data processing, filtering (you do not need this file if you call dsp's function to complete)
- __dsp_g2.c__ (Test sequence generation function, Fast Fourier Transform call function, serial communication functions, all encapsulated in a function, inside the main function can be called.btw,this is their own implementation of the FFT algorithm)
- __dsp_asm.c__ （Ibid, but this is a direct call library function FFT algorithm is not recommended）




			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
