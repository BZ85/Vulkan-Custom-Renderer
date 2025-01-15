# Vulkan-Custom-Renderer
## Author: Xinjie Zhu
### Project Description
This is my custom Vulkan renderer, and I follow the Vulkan tutorial to build it. The renderer is built by Vulkan API from scratch, and the basic features of Vulkan API like swap chain images, graphics pipeline setting, descriptor sets, push constants, depth buffer, textures, model loading are implemented. The lighting is also implemented in the my Vulkan renderer by Phong Shading. The video shows an example scene (an aircraft in the space) of my renderer with lighting.

The program also supports basic animation (without skeleton) by transformation and user interactions. The user can use mouse control to rotate the object as shown in the demo video. During the rotation, the lighting effect is also demonstrated. User interactions and windows are implemented by GLFW.

 <a href="https://drive.google.com/file/d/1VJtc-ZmQ090zXYX0K4FFk18HsWpiAg9_/view" table = "_blank"> Demo Video Link </a>

 
 <img src="./pictures/demoPicture.png" alt="alt text" width="70%" height="70%"/> 
 </br>
  </br>


The program can take as the obj file and mtl file as input, and load the extra model with textures. Open Asset Import Library (Assimp) library is used to implement this feature.

Although the implemented graphics features are not complicated, it shows my ability to understand and work with Vulkan's low-level API. Vulkan's complexity, compared to higher-level APIs like OpenGL or DirectX11, makes it significantly more challenging to grasp. This project demonstrates my ability in modern graphics programming and familiarity with the low-level working of GPU. More features will be added in this Vulkan renderer in the future.

Mouse Control (should stop the animation first): 
Rotation: X axis: press left mouse button and dragging vertically
          Y axis: press right mouse button and dragging horizontally 
          Z axis: press left mouse button and dragging horizontally

Animation: press space bar to start or stop

### References
The resource of the 3D model is from https://free3d.com/3d-model/e-45-aircraft-71823.html
Vulkan Tutorial: https://vulkan-tutorial.com/

