# NTNU_ACG
2020 NTNU資工碩 Advanced Computer Graphics(ray trace)  
teacher:張鈞法 

## LAB 1
根據給定座標(in hw1_input.txt)，render出圖片  
僅考慮是否遮擋  

E : 眼睛(camera)座標  
O : 視野邊界(左上 / 右上 / 左下 / 右下)  
R : 解析度(resolution) 寬/高  
S : 球體圓心座標, 半徑  
T : 三角形各頂點座標  

## LAB 2
根據給定座標(in hw2_input.txt)，render出圖片   
考慮光影問題  

E : 眼睛(camera)座標  
V : 視野方向(視線方向 / 正上方向量)  
F : 視野(FoV)  
R : 解析度(resolution) 寬/高    
M : 材質係數(ambient / diffuse / specular / reflect)  
S : 球體圓心座標, 半徑  
T : 三角形各頂點座標  
L : 光源方向

## LAB 3
多物體，考慮對焦問題   

E : 眼睛(camera)座標  
V : 視野方向(視線方向 / 正上方向量)  
F : 視野(FoV)  
R : 解析度(resolution) 寬/高    
M : 材質係數(ambient / diffuse / specular / reflect)  
S : 球體圓心座標, 半徑  
T : 三角形各頂點座標  
L : 光源方向 
 
## final project
考慮不同反射公式  
反射後全散射  
![image](https://github.com/wl03036530/NTNU_ACG/blob/main/1.png)  
反射後往光源方向散射(有目的性散射)  
![image](https://github.com/wl03036530/NTNU_ACG/blob/main/2.png)
### 探討不同反射公式造成的影響
在final資料夾中只render光的顏色  
**圖中純黑的地方是光源永遠不會打進去的點**

Reference: https://raytracing.github.io/books/RayTracingInOneWeekend.html
