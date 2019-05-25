# Выбор решения

### google-запросы

* streaming algorithm distinct elements

* distinct elements in data stream

* cardinality estimation data steam

### Полученные статьи

* Обзоры по теме: [(2017, Sedgewick)](https://www.cs.princeton.edu/~rs/talks/Cardinality.pdf), 
	[(2017, Harmouch, Naumann)](http://www.vldb.org/pvldb/vol11/p499-harmouch.pdf)

	У Седжвика в конце описан алгоритм **HyperBitBit**. Теоретический анализ алгоритма, судя по [отсутствию](https://arxiv.org/search/?searchtype=all&query=Sedgewick&abstracts=show&size=50&order=-submitted_date)
	свежих публикаций по теме от Роберта Седжвика, на момент 24 мая 2019 не опубликован.

	Во втором обзоре описаны **minCount** и *AKMV*, которые оказались близки к моим идеям.

* Общие слова по теме:

	https://en.wikipedia.org/wiki/Count-distinct_problem

	https://www.cs.ubc.ca/~nickhar/W15/Lecture15Notes.pdf

	https://people.eecs.berkeley.edu/~satishr/cs270/sp11/rough-notes/Streaming.pdf


* Основной теоретической работой в области является алгоритм **HyperLogLog** из статьи

	(2002) Authors: {Ziv Bar-Yossef, Jayram, Kumar et al}, [Counting Distinct Elements in a Data Stream](https://dl.acm.org/citation.cfm?id=711822)

* Оптимальный теоретический результат.

	(2010) Authors: {Kane, Nelson, Woodruff}, [An Optimal Algorithm for the Distinct Elements Problem](http://www.cs.cmu.edu/afs/cs/user/dwoodruf/www/knw10b.pdf)

* О тонкостях практических применений **HyperLogLog** известно из

	(2007) Authors: {Flajolet, et al}, [HyperLogLog: the analysis of a near-optimal cardinality estimation algorithm](http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf)

	(2013) Authors: {Heule, Nunkesser, Hall}, [HyperLogLog in Practice](https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/40671.pdf)

* Классическая теоретическая работа об $F_p$ для $p > 0$ (мы сейчас считаем $F_0$).

	(2010) Authors: {Kane, Nelson, Woodruff}, [Fast Moment Estimation in Data Streams in Optimal Space](https://arxiv.org/pdf/1007.4191.pdf)

* Дополнительно можно смотреть на работы, цитирующие основные статьи от [2002](https://dl.acm.org/citation.cfm?id=711822) и
	[2010](https://dl.acm.org/citation.cfm?id=1807094).<br>
	Также https://arxiv.org/search/ даёт смежные работы по теме: [(1)](https://arxiv.org/abs/1810.12388),
	[(2)](https://arxiv.org/abs/1804.01642), [(3)](https://arxiv.org/abs/1402.6800).

### Выбранные решения, существующие реализации

* Из ограниченности времени принято решение сравнить вариации **HyperLogLog**, **HyperBitBit**, **AKMV**.

* Реализации HyperBitBit на github: 
[(cpp | dnbaker/sketch)](https://github.com/dnbaker/sketch/blob/master/hbb.h),
[(go)](https://github.com/seiflotfy/hyperbitbit), 
[(java)](https://github.com/addthis/stream-lib/blob/master/src/main/java/com/clearspring/experimental/stream/cardinality/HyperBitBit.java).

* Реализации HyperLogLog на github 
[(cpp)](https://github.com/hideo55/cpp-HyperLogLog/blob/master/include/hyperloglog.hpp),
[(go)](https://github.com/axiomhq/hyperloglog/blob/master/hyperloglog.go), 
[(java)](https://github.com/prasanthj/hyperloglog/blob/master/src/java/com/github/prasanthj/hll/HyperLogLog.java), 
[(cs | microsoft)](https://github.com/microsoft/CardinalityEstimation/blob/master/CardinalityEstimation/CardinalityEstimator.cs).

# Мои реализации

По [условию](statement.txt) нам дают 32К памяти, структура данных должна оперировать с 32-битными целыми числами. 

## Решения на основе поддержки k-й статистики

Метод придумался как-то сразу, гугление показало, что у него есть название "AKMV", см. [обзор](http://www.vldb.org/pvldb/vol11/p499-harmouch.pdf).

### [ucFirstApproach.h](ucFirstApproach.h) 

* Хранение set-а $n$ = 32К / sizeof(set::node) = 2048 минимальных чисел

* Результат выражается из $n$-й порядковой статистики

* Итого: $10^6$ запросов за ~0.08 секунд, средняя погрешность в пределах 1.6%, максимальная 4.1%.

### [ucHeapAKMV.h](ucHeapAKMV.h) 

* Попробуем заменить <code>set</code> на <code>heap</code>. 

* Эффект 1: можем хранить в 4 раза больше элементов, меньше погрешность, см. [experimentResult.txt](experimentResult.txt).

* Эффект 2: для $10^5$ запросов работает в ~25-30 раз дольше.

* Итого: $10^6$ запросов за ~2.8 секунд, средняя погрешность в пределах 1%, максимальная 3.3%.

### Beap

Компромиссом между <code>set</code> и <code>heap</code>, возможно, будет реализация inplace set.

Структура называется [<code>beap</code>](https://core.ac.uk/download/pdf/82247166.pdf), занимает памяти столько же, <code>heap</code>, умеет запросы <code>heap</code> + поиск за $O(n^{1/2})$.

## Решения на основе HyperLogLog, HyperBitBit

* Если коротко, не работают =)
