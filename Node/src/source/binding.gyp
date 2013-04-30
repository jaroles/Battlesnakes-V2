{
  "targets": [
    {
      "target_name": "collision",
      "sources": [ "collision.cpp", "CubicBezierSegment.cpp", "Point.cpp", "Vector.cpp"],
      "include_dirs": [ "C:\\Boost\\boost_1_53_0" ],
      "link_settings": {
      	"library_dirs": [ "C:\\Boost\\boost_1_53_0\\stage\\lib" ]
      },
    },
	{
      "target_name": "miniSnakeAddon",
      "sources": [ "miniSnakeAddon.cpp", "Bush.cpp", "EnvironmentObject.cpp", "GameObject.cpp", "Grid.cpp", "GridController.cpp", "GridSection.cpp", "Hatchery.cpp", "MiniSnake.cpp", "Point.cpp", "Rock.cpp", "Teams.cpp", "Vector.cpp"],
      "include_dirs": [ "C:\\Boost\\boost_1_53_0" ],
      "link_settings": {
      	"library_dirs": [ "C:\\Boost\\boost_1_53_0\\stage\\lib" ]
      },
    },
  ],
}