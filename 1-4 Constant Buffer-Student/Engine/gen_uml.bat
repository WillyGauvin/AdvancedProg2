rename pch.h pch.foo
rename pch.cpp pch.bar
hpp2plantuml -d --input-file "*.h" -o UML.puml
rename pch.foo pch.h
rename pch.bar pch.cpp