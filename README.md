## 关于FFT代码移植的要点：
#### 首先，我们需要移植文件：

> 1.table_fft.h (FFT表文件)
- stm32_dsp.h (调用DSP库函数的头文件，如果是自己编写的fft，则不需要此文件)
- cr4_fft_64_stm32.s 和 cr4_fft_256_stm32.s 和 cr4_fft_1024_stm32.s （DSP傅里叶变换的可执行函数，如果是自己完成的fft则不需要此文件）
- FFT2.c（和同一目录下FFT2)  这个是自己实现FFT的函数所在文件，其中包含简单的数据处理，滤波（如果是调用dps函数完成，则不需要此文件）
- dsp_g2.c (测试序列发生函数，fft调用函数，串口通信函数，全部封装在一个函数内，在main函数里面调用即可，这个是自己实现的FFT)
- dsp_asm.c （同上，不过这个是直接调用库函数的FFT，不推荐使用）




			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			