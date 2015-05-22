# Develop log #

Currently i donot know where to put these develop log for edge2d. It doesnot matter that i post them here.

# Details #

##### 10.25.2007 #####

0.1.0 will be released soon. Today i port CrazyEggs to edge2d. And everything goes well except there is a bug on Ati video card. But trust me it's not the bug of edge. I should consider that it's the bug for graphics plugin . As my introduction for edge, edge only contains abstract code and basic code. Everything will implement something is a plugin. So the graphics module also is a plugin. i think it's better for me to fix the bug.

BTW, i asked some friends to make logo for edge. i hope i can get the best logo for edge at last.

##### 10.26.2007 #####

There still has a bug on some ATI video cards. The texture cannot be rendered correctly. I donot know why and donot know how to debug this bug. So i upload D3D8Graphics plugin source code. I hope somebody can fix it .

##### 10.26.2007 15:08 #####

I have fixed the bug talked before. The reason is the difference between nvidia card and ati card. When i create a d3d texture and the size is not a power of 2, nvidia will create it if the card support. But on ati card, it's difference, it will pad the size to be a power of 2 automatically. Maybe it's the difference that D3DXCreateTexture performes on nvidia and ati card. Anyway, i always use the texture original size. So that works correctly.

I upload the new version of CrazyEggsEdge.

##### 10.31.2007 #####

Today i commit all the source code and vs2005 binary file to this svn server. And i have already written some examples so that you can learn quickly.

And currently the svn contains  :
  * full source code including EdgeBase and all plugin source,
  * binray files compiled by vs2005,
  * an api document compiled chm version which is created automatically by doxygen,
  * 6 examples from the basic hello world application to a more complicated one,
  * lzo tool to create lzo archive and animation tool to create an animation file

##### 11.4.2007 #####

Today sea\_bug built an online forum for Edge2d. I appreciate he did this for me for Edge. Thank you sea\_bug. And the forum url is : http://bbs.gameres.com/showforum.asp?forumid=91 (A Chinese forum)

##### 11.5.2007 #####

I ported Popo game to Edge today. And you can download the game and the source code here:
http://edge2d.googlecode.com/files/EdgeExample_Popo_Full.zip