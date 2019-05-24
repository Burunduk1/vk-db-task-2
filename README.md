# Выбор решения

### google-запросы

* streaming algorithm distinct elements

* distinct elements in data stream

* cardinality estimation data steam

### Полученные статьи

* Крутой обзор по теме:

	https://www.cs.princeton.edu/~rs/talks/Cardinality.pdf

	https://www.cs.princeton.edu/~rs/talks/AC11-Cardinality.pdf

	В конце описан алгоритм **HyperBitBit**. Теоретический анализ алгоритма, судя по [отсутствию](https://arxiv.org/search/?searchtype=all&query=Sedgewick&abstracts=show&size=50&order=-submitted_date)
	свежих публикаций по теме от Роберта Седжвика, на момент 24 мая 2019 не опубликован.


* Общие слова по теме:

	https://en.wikipedia.org/wiki/Count-distinct_problem

	https://www.cs.ubc.ca/~nickhar/W15/Lecture15Notes.pdf

	https://people.eecs.berkeley.edu/~satishr/cs270/sp11/rough-notes/Streaming.pdf

* Основной теоретической работой в области является алгоритм **HyperLogLog** из статьи

	(2002) Authors: {Ziv Bar-Yossef, T. S. Jayram, Ravi Kumar, D. Sivakumar, Luca Trevisan}, 

	Counting Distinct Elements in a Data Stream, https://dl.acm.org/citation.cfm?id=711822

* О тонкостях практических применений **HyperLogLog** известно из

	(2013) Authors: {S. Heule, M. Nunkesser, A. Hall}, HyperLogLog in Practice, https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/40671.pdf

* Классическая теоретическая работа об $F_p$ для $p > 0$ (мы сейчас считаем $F_0$).

	(2010) Authors: {Kane, Nelson and Woodruff}, Fast Moment Estimation in Data Streams in Optimal Space, https://arxiv.org/pdf/1007.4191.pdf

* Дополнительно можно смотреть на работы, цитирующие [основную статью](https://dl.acm.org/citation.cfm?id=711822),

	и https://arxiv.org/search/, смежные работы по теме: [(1)](https://arxiv.org/abs/1810.12388),
	[(2)](https://arxiv.org/abs/1804.01642), [(3)](https://arxiv.org/abs/1402.6800).

### Выбранные решения, существующие реализации

* Из ограниченности времени принято решение использовать только базовые **HyperLogLog** и **HyperBitBit**.

* Реализации HyperBitBit на github: [(1)](https://github.com/seiflotfy/hyperbitbit), [(2)](https://github.com/addthis/stream-lib/blob/master/src/main/java/com/clearspring/experimental/stream/cardinality/HyperBitBit.java).

* Реализации HyperLogLog на github [(1)](https://github.com/axiomhq/hyperloglog), [(2)](https://github.com/prasanthj/hyperloglog), [(microsoft)](https://github.com/microsoft/CardinalityEstimation).

# Моя реализация

По [условию](statement.txt) нам дают 32К памяти, структура данных должна оперировать с 32-битными целыми числами. 

