#ifndef   GUI_WXWIDGETS_MAIN_APP_TABLE_VIEW_EX_H_
#define   GUI_WXWIDGETS_MAIN_APP_TABLE_VIEW_EX_H_

#include <cstdint>
#include <array>
#include <map>
#include <functional>

#ifdef _MSC_VER
#define snprintf _snprintf_s
#endif

/*****************************************************************************
 *
 * STRUCT  : ColumnInfo
 * PURPOSE : Template-based structure for storing column metadata
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: In the future, ColumnType needs to be supplemented as needed
 *
 *****************************************************************************/

template <typename C>
struct ColumnInfo
{
  // Column data type
  enum class ColumnType
  {
    INT32,
    INT64,
    UINT32,
    UINT64,
    FLOAT,
    DOUBLE,
    STRING,
    WSTRING
  };
  // Stores the data type of the current column
  ColumnType                       type;
  // Format string for printf-style formatting
  std::string                      format;
  // Function to filter values
  std::function<bool(const void*)> filter;
  // Extra metadata for this column
  C                                extraInfo;
  // Column name
  std::string                      name;
};

/*****************************************************************************
 *
 * STRUCT  : ColumnData
 * PURPOSE : Multi-type variable data storage structure based on union
             implementation
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: More data types will be supported in the future as needed
 *
 *****************************************************************************/

template <typename C>
struct ColumnData
{
  using ColumnType = typename ColumnInfo<C>::ColumnType;

  // Data type of this column
  ColumnType            type;
  // Pointer to column metadata
  const ColumnInfo<C>*  columnInfo = nullptr;
  // Storing actual data
  union
  {
    int32_t        i32;
    int64_t        i64;
    uint32_t       u32;
    uint64_t       u64;
    float          f;
    double         d;
  }                value;    // Storing numeric data
  std::string      str;      // Storing       string
  std::wstring     wstr;     // Storing wide  string

  // Constructors
  ColumnData(                         ) : type(ColumnType::INT32      ) { value.i32    = 0;  }
  ColumnData(int32_t                 v) : type(ColumnType::INT32      ) { value.i32    = v;  }
  ColumnData(int64_t                 v) : type(ColumnType::INT64      ) { value.i64    = v;  }
  ColumnData(uint32_t                v) : type(ColumnType::UINT32     ) { value.u32    = v;  }
  ColumnData(uint64_t                v) : type(ColumnType::UINT64     ) { value.u64    = v;  }
  ColumnData(float                   v) : type(ColumnType::FLOAT      ) { value.f      = v;  }
  ColumnData(double                  v) : type(ColumnType::DOUBLE     ) { value.d      = v;  }
  ColumnData(const std::string&      v) : type(ColumnType::STRING     ),      str       (v) {}
  ColumnData(const std::wstring&     v) : type(ColumnType::WSTRING    ),      wstr      (v) {}

  // Format value based on column format
  std::string FormatValue() const
  {
    if (type == ColumnType::STRING)
      return str;
    if (type == ColumnType::WSTRING)
      return std::string(wstr.begin(), wstr.end());
    if (!columnInfo)
      return "";

    const size_t     BUFFER_SIZE = 256;
    char      buffer[BUFFER_SIZE];

    snprintf(buffer, BUFFER_SIZE, columnInfo->format.c_str(),
      (type == ColumnType::FLOAT      ) ? value.f      :
      (type == ColumnType::DOUBLE     ) ? value.d      :
      (type == ColumnType::UINT32     ) ? value.u32    :
      (type == ColumnType::UINT64     ) ? value.u64    :
      (type == ColumnType::INT64      ) ? value.i64    :
                                          value.i32   );
    return std::string(buffer);
  }

  // Format value for wide character output
  std::wstring FormatValueW() const
  {
    if (type == ColumnType::STRING)
      return std::wstring(str.begin(), str.end());
    if (type == ColumnType::WSTRING)
      return wstr;
    if (!columnInfo)
      return L"";

    constexpr size_t BUFFER_SIZE = 256;
    wchar_t   buffer[BUFFER_SIZE];

    swprintf(buffer, BUFFER_SIZE,
      std::wstring(columnInfo->format.begin(), columnInfo->format.end()).c_str(),
      (type == ColumnType::FLOAT      ) ? value.f      :
      (type == ColumnType::DOUBLE     ) ? value.d      :
      (type == ColumnType::UINT32     ) ? value.u32    :
      (type == ColumnType::UINT64     ) ? value.u64    :
      (type == ColumnType::INT64      ) ? value.i64    :
                                          value.i32   );
    return std::wstring(buffer);
  }
};

/*****************************************************************************
 *
 * STRUCT  : TableEx
 * PURPOSE : Template-based table element processing class
 * AUTHOR  : Peixuan Zhang <Zhang@PoeticFlower.CN>
 * COMMENTS: Support batch creation, updating, sorting and filtering
 *
 *****************************************************************************/

template <typename C, size_t N>
class TableEx
{
public:
  using RowData = std::array<ColumnData<C>, N>;
protected:
  // Column metadata
  std::array<ColumnInfo<C>, N>     m_arrColumnInfo;
  // Store rows using a map for ID lookup
  std::map<size_t, RowData>        m_vRows;
  // Cached sorted rows
  std::vector<RowData>             m_vSortedRows;
  // Indicates if sorting is valid
  bool                             m_bSortedValid = false;
public:
  // Get column metadata by index (returns reference to avoid copy overhead)
  const ColumnInfo<C>& GetColumnInfo(size_t col) const
  {
    static ColumnInfo<C> dummyColumn = {};
    return (col < N) ? m_arrColumnInfo[col] : dummyColumn;
  }
  // Set column metadata
  void SetColumnInfo(size_t col, const ColumnInfo<C>& info)
  {
    if (col < N)
    {
      m_arrColumnInfo[col] = info;
    }
  }
  // Set filter function for a specific column
  void SetFilter(size_t col, std::function<bool(const void*)> filter)
  {
    if (col < N)
    {
      m_arrColumnInfo[col].filter = filter;
    }
  }
  // Clear filter for a specific column or all columns if col is out of range
  void ClearFilter(size_t col = -1)
  {
    if (col < N)
    {
      m_arrColumnInfo[col].filter = nullptr;
    }
    else
    {
      for (auto& colInfo : m_arrColumnInfo)
      {
        colInfo.filter = nullptr;
      }
    }
  }
  // Insert or update a row
  void UpsertRow(size_t id, std::array<ColumnData<C>, N> row)
  {
    // Link column info
    for (size_t i = 0; i < N; ++i)
    {
      row[i].columnInfo = &m_arrColumnInfo[i];
    }

    m_vRows[id] = row;

    // Invalidate sorted data
    m_bSortedValid = false;
  }
  // Sort rows by a specific column
  void SortByColumn(size_t col, bool ascending = true)
  {
    std::vector<std::pair<size_t, RowData>> sortedRows(
      m_vRows.begin(), m_vRows.end());

    std::sort(
      sortedRows.begin(),
      sortedRows.end(),
      [col, ascending](
        const std::pair<size_t, RowData>& a,
        const std::pair<size_t, RowData>& b)
    {
      const ColumnData<C>& left   = a.second[col];
      const ColumnData<C>& right  = b.second[col];

      switch (left.type)
      {
      case ColumnInfo<C>::ColumnType::INT32:
        return ascending ? left.value.i32   < right.value.i32
                         : left.value.i32   > right.value.i32;
      case ColumnInfo<C>::ColumnType::INT64:
        return ascending ? left.value.i64   < right.value.i64
                         : left.value.i64   > right.value.i64;
      case ColumnInfo<C>::ColumnType::UINT32:
        return ascending ? left.value.u32   < right.value.u32
                         : left.value.u32   > right.value.u32;
      case ColumnInfo<C>::ColumnType::UINT64:
        return ascending ? left.value.u64   < right.value.u64
                         : left.value.u64   > right.value.u64;
      case ColumnInfo<C>::ColumnType::FLOAT:
        return ascending ? left.value.f     < right.value.f
                         : left.value.f     > right.value.f;
      case ColumnInfo<C>::ColumnType::DOUBLE:
        return ascending ? left.value.d     < right.value.d
                         : left.value.d     > right.value.d;
      case ColumnInfo<C>::ColumnType::STRING:
        return ascending ? left.str         < right.str
                         : left.str         > right.str;
      case ColumnInfo<C>::ColumnType::WSTRING:
        return ascending ? left.wstr        < right.wstr
                         : left.wstr        > right.wstr;
      default:
        // Unsupported sort type
        return false;
      }
    });

    m_vSortedRows.clear();
    for (const auto& pair : sortedRows)
    {
      m_vSortedRows.push_back(pair.second);
    }

    m_bSortedValid = true;
  }
  // Iterate over rows and apply a function
  void ForEach(std::function<void(const RowData&)> func) const
  {
    if (m_bSortedValid)
    {
      for (const auto& row : m_vSortedRows)
      {
        bool filtered = false;
        for (size_t i = 0; i < N; ++i)
        {
          if (  m_arrColumnInfo[i].filter
            && !m_arrColumnInfo[i].filter(&row[i]))
          {
            filtered = true;
            break;
          }
        }
        if (!filtered)
        {
          func(row);
        }
      }
    }
    else
    {
      for (const auto& pair : m_vRows)
      {
        const auto& row = pair.second;
        bool filtered = false;
        for (size_t i = 0; i < N; ++i)
        {
          if (  m_arrColumnInfo[i].filter
            && !m_arrColumnInfo[i].filter(&row[i]))
          {
            filtered = true;
            break;
          }
        }
        if (!filtered)
        {
          func(row);
        }
      }
    }
  }
};

#endif // GUI_WXWIDGETS_MAIN_APP_TABLE_VIEW_EX_H_
